#include "plcCore.hpp"

PLCCore::PLCCore() {}

void PLCCore::init()
{
    this->IO = new IOMonitor();
    this->DB = new DataBase();
    this->IO->init();
    this->DB->init();
}

void PLCCore::bootInfo() {
    taskUpdateOPConfig();
}

void PLCCore::initRTC()
{
    Global::EE.readEEPROM(isTimeSetBit_EEAddr, &EEPROMresult);
    if (EEPROMresult != 1)
    {
        Global::EE.writeEEPROM(isTimeSetBit_EEAddr, EEPROMresult = 1);
        DB->SCR6_GETSET_Day.setAndWriteToEE(11);
        DB->SCR6_GETSET_Month.setAndWriteToEE(11);
        DB->SCR6_GETSET_Year.setAndWriteToEE(11);
        DB->SCR6_GETSET_RTCHH.setAndWriteToEE(11);
        DB->SCR6_GETSET_RTCMM.setAndWriteToEE(11);
    }
    
    rtc.setTime(DB->SCR6_GETSET_RTCHH.getValue(), DB->SCR6_GETSET_RTCMM.getValue(), 00);
    rtc.setDate(DB->SCR6_GETSET_Day.getValue(), DB->SCR6_GETSET_Month.getValue(), DB->SCR6_GETSET_Year.getValue());
}

void PLCCore::setNewRTC()
{
    DB->SCR6_GETSET_Day.fromOPtoEE();
    DB->SCR6_GETSET_Month.fromOPtoEE();
    DB->SCR6_GETSET_Year.fromOPtoEE();
    DB->SCR6_GETSET_RTCHH.fromOPtoEE();
    DB->SCR6_GETSET_RTCMM.fromOPtoEE();
    DB->SCR6_GETSET_AlarmHH.fromOPtoEE();
    DB->SCR6_GETSET_AlarmMM.fromOPtoEE();

    rtc.setTime(DB->SCR6_GETSET_RTCHH.getValue(), DB->SCR6_GETSET_RTCMM.getValue(), rtc.getSeconds());
    rtc.setDate(DB->SCR6_GETSET_Day.getValue(), DB->SCR6_GETSET_Month.getValue(), DB->SCR6_GETSET_Year.getValue());
    rtc.setAlarmTime(DB->SCR6_GETSET_AlarmHH.getValue(), DB->SCR6_GETSET_AlarmMM.getValue(), 00);
    taskUpdateOPConfig();
}

void PLCCore::initAlarm()
{
    Global::EE.readEEPROM(isAlarmSetBit_EEAddr, &EEPROMresult);
    if (EEPROMresult != 1)
    {
        Global::EE.writeEEPROM(isAlarmSetBit_EEAddr, EEPROMresult = 1);
        DB->SCR6_GETSET_AlarmHH.setAndWriteToEE(6);
        DB->SCR6_GETSET_AlarmMM.setAndWriteToEE(30);
    }

    DB->SCR6_AlarmMode.readFromEE();
    if (DB->SCR6_AlarmMode.getValue() != 0)
        rtc.setAlarmTime(DB->SCR6_GETSET_AlarmHH.getValue(), DB->SCR6_GETSET_AlarmMM.getValue(), 0);
}

void PLCCore::initConfig()
{
    if (DB->SCR3_BlowGunVolume.getValue() <= 0)
        DB->SCR3_BlowGunVolume.setAndWriteToEE(1);

    if (DB->SCR9_GETSET_LCDfreeq.getValue() < 3)
        DB->SCR9_GETSET_LCDfreeq.setAndWriteToEE(3);
    
    if (DB->SCR10_GETSET_PasteurDuratMM.getValue() < 30)
        DB->SCR10_GETSET_PasteurDuratMM.setAndWriteToEE(30);
    
    if (DB->SCR10_GETSET_HeatingDuratMM.getValue() < 30)
        DB->SCR10_GETSET_HeatingDuratMM.setAndWriteToEE(30);

    if (DB->SCR10_GETSET_PasteurTempC.getValue() < 60)
        DB->SCR10_GETSET_PasteurTempC.setAndWriteToEE(60);

    if (DB->SCR10_GETSET_HeatingTempC.getValue() < 30)
        DB->SCR10_GETSET_HeatingTempC.setAndWriteToEE(30);

    if (DB->SCR11_GETSET_BlowGunCalibMS.getValue() <= 0)
        DB->SCR11_GETSET_BlowGunCalibMS.setAndWriteToEE(1);
}

// checking EEPROM for isTaskInProccessBit means that on power off PLC run program or not
void PLCCore::initState()
{
    /*
    Global::EE.readEEPROM(isTaskInProccessBit_EEAddr, &Current_TaskInProccess);
    if (Current_TaskInProccess != 0)
    {
        Global::EE.readEEPROM(TaskProccessStep_EEAddr, &Current_TaskProccessStep);
        Global::EE.readEEPROM(TaskStepEstimatedMin_EEAddr, &Current_EstimatedMin);
        
        //вызов функции программы пастеризации
        
    }
    */
}

void PLCCore::microReadCommands()
{
    if (DB->SCR1_COMM_ESC_ToCancelScreen.readFromOP() != 0)
    {
        DB->SCR1_COMM_ESC_ToCancelScreen.setAndWriteToOP(0);
        if (Current_TaskInProccess == 1 || Current_WashingTask || Current_MixingTask)
            DB->GLOBAl_OPScreen.setAndWriteToOP(2);
    }

    if (DB->SCR2_COMM_9_AcceptCancelling.readFromOP() != 0)
    {
        DB->SCR2_COMM_9_AcceptCancelling.setAndWriteToOP(0);
        DB->GLOBAl_OPScreen.setAndWriteToOP(1);
        this->emergencyStop();
    }

    if (DB->SCR3_COMM_UP_BlowgunPlus250.readFromOP() != 0)
    {
        DB->SCR3_COMM_UP_BlowgunPlus250.setAndWriteToOP(0);
        DB->SCR3_BlowGunVolume.setAndWriteToEE(DB->SCR3_BlowGunVolume.getValue() + 1);
        DB->SCR3_BlowGunVolume.writeToOP();
    }

    if (DB->SCR3_COMM_DOWN_BlowgunMin250.readFromOP() != 0)
    {
        DB->SCR3_COMM_DOWN_BlowgunMin250.setAndWriteToOP(0);
        DB->SCR3_BlowGunVolume.setAndWriteToEE(DB->SCR3_BlowGunVolume.getValue() - 1);
        DB->SCR3_BlowGunVolume.writeToOP();
    }

    if (DB->SCR3_COMM_FreeButt_StartBlowgun.readFromOP() != 0)
    {
        DB->SCR3_COMM_FreeButt_StartBlowgun.setAndWriteToOP(0);
        taskBlowGunDosage();
    }

    if (DB->SCR6_COMM_UPDOWN_SwitchAlarmMode.readFromOP() != 0)
    {
        DB->SCR6_COMM_UPDOWN_SwitchAlarmMode.setAndWriteToOP(0);
        DB->SCR6_AlarmMode.setAndWriteToEE(DB->SCR6_AlarmMode.getValue() == 0 ? (uint8_t)1 : (uint8_t)0);
        DB->SCR6_AlarmMode.writeToOP();
    }
    
    if (DB->SCR6_COMM_SET_AcceptNewOPDateTime.readFromOP() != 0)
    {
        DB->SCR6_COMM_SET_AcceptNewOPDateTime.setAndWriteToOP(0);
        setNewRTC();
    }

    if (DB->SCR9_COMM_SET_AcceptNewDataFreeq.readFromOP() != 0)
    {
        DB->SCR9_COMM_SET_AcceptNewDataFreeq.setAndWriteToOP(0);
        DB->SCR9_GETSET_LCDfreeq.fromOPtoEE();
        Global::Freeq = DB->SCR9_GETSET_LCDfreeq.getValue();
    }

    if (DB->SCR10_COMM_SET_AcceptNewPHParams.readFromOP() != 0)
    {
        DB->SCR10_COMM_SET_AcceptNewPHParams.setAndWriteToOP(0);
        DB->SCR10_GETSET_PasteurDuratMM.fromOPtoEE();
        DB->SCR10_GETSET_HeatingDuratMM.fromOPtoEE();
        DB->SCR10_GETSET_PasteurTempC.fromOPtoEE();
        DB->SCR10_GETSET_HeatingTempC.fromOPtoEE();
    }
    
    if (DB->SCR11_COMM_SET_AcceptBlowgunCalib.readFromOP() != 0)
    {
        DB->SCR11_COMM_SET_AcceptBlowgunCalib.setAndWriteToOP(0);
        DB->SCR11_GETSET_BlowGunCalibMS.fromOPtoEE();
    }

    if (DB->SCR11_COMM_TestBlowgunCalib.readFromOP() != 0)
    {
        DB->SCR11_COMM_TestBlowgunCalib.setAndWriteToOP(0);
        IO->toggleRelay(BlowGunPin, true);
        delay(DB->SCR11_GETSET_BlowGunCalibMS.getValue() * 100);
        IO->toggleRelay(BlowGunPin, false);
    }

    if (DB->SCR1_COMM_ALM_ONOFFMixer.readFromOP() != 0)
    {
        DB->SCR1_COMM_ALM_ONOFFMixer.setAndWriteToOP(0);
        if (!this->commMixerRotate(true))
            DB->GLOBAl_OPScreen.setAndWriteToOP(2);
        else
            DB->GLOBAl_OPScreen.setAndWriteToOP(1);
    }

    if (DB->SCR4_COMM_2_StartWashingProgramm.readFromOP() != 0)
    {
        DB->SCR4_COMM_2_StartWashingProgramm.setAndWriteToOP(0);
        if (!this->commWaterPump(true))
            DB->GLOBAl_OPScreen.setAndWriteToOP(2);
        else
            DB->GLOBAl_OPScreen.setAndWriteToOP(1);
    }

    if (DB->SCR4_COMM_1_StartPasteurProgramm.readFromOP() != 0)
    {
        DB->SCR4_COMM_1_StartPasteurProgramm.setAndWriteToOP(0);
        if (!bootOP320PasteurError)
        {
            if (!this->callPasteurization(0))
                DB->GLOBAl_OPScreen.setAndWriteToOP(2);
            else
                DB->GLOBAl_OPScreen.setAndWriteToOP(1);
        }
        else
            bootOP320PasteurError = false;
    }
}

void PLCCore::taskUpdateOPInfo()
{
    uint8_t Old_EstimatedMM = Current_EstimatedTimeMM;
    uint8_t Old_HeatingMM = Current_EstimatedHeatMM;

    if (rtc.getHours() != DB->SCR6_GETSET_RTCHH.getValue())
    {
        DB->SCR6_GETSET_RTCHH.setAndWriteToEE(rtc.getHours());
        DB->SCR6_GETSET_RTCHH.fromEEtoOP();
        DB->SCR6_GETSET_RTCMM.fromEEtoOP();
        if (Current_TaskInProccess == 1 && Current_EstimatedTimeMM > 0)
            --Current_EstimatedTimeMM;
        if (Current_TaskFinished == 1 && Current_EstimatedHeatMM > 0)
            --Current_EstimatedHeatMM;
    }

    if (rtc.getMinutes() != DB->SCR6_GETSET_RTCMM.getValue())
    {
        rtc.setSubSeconds(rtc.getSubSeconds() + 500);
        DB->SCR6_GETSET_RTCMM.setAndWriteToEE(rtc.getMinutes());
        DB->SCR6_GETSET_RTCMM.fromEEtoOP();
        DB->SCR6_GETSET_RTCHH.fromEEtoOP();
        if (Old_EstimatedMM == Current_EstimatedTimeMM && Current_TaskInProccess != 0 && Current_EstimatedTimeMM > 0)
            --Current_EstimatedTimeMM;
        if (Old_HeatingMM == Current_EstimatedHeatMM && Current_TaskFinished == 1 && Current_EstimatedHeatMM > 0)
            --Current_EstimatedHeatMM;
    }

    if (Global::PLCCore_SerialLog)
    {
        Serial.println(Global::EE_currSectCyclCnt);
        Serial.print(DB->SCR6_GETSET_RTCHH.getValue());
        Serial.print(":");
        Serial.println(DB->SCR6_GETSET_RTCMM.getValue());
    }
}

void PLCCore::taskUpdateOPConfig()
{
    //rtc
    DB->SCR6_GETSET_RTCHH.writeToOP();
    DB->SCR6_GETSET_RTCMM.writeToOP();
    DB->SCR6_GETSET_Day.writeToOP();
    DB->SCR6_GETSET_Month.writeToOP();
    DB->SCR6_GETSET_Year.writeToOP();

    //alarm
    DB->SCR6_GETSET_AlarmHH.writeToOP();
    DB->SCR6_GETSET_AlarmMM.writeToOP();

    //config
    DB->SCR3_BlowGunVolume.writeToOP();
    DB->SCR9_GETSET_LCDfreeq.writeToOP();
    DB->SCR10_GETSET_PasteurDuratMM.writeToOP();
    DB->SCR10_GETSET_HeatingDuratMM.writeToOP();
    DB->SCR10_GETSET_PasteurTempC.writeToOP();
    DB->SCR10_GETSET_HeatingTempC.writeToOP();
    DB->SCR11_GETSET_BlowGunCalibMS.writeToOP();

    Global::Freeq = DB->SCR9_GETSET_LCDfreeq.getValue() > 3 ? DB->SCR9_GETSET_LCDfreeq.getValue() : 5;
}

void PLCCore::taskBlowGunDosage()
{
    //DB->SCR11_GETSET_BlowGunCalibMS.getValue() * 100 / 500 * DB->SCR3_BlowGunVolume.getValue(),
    //delay(8000);
    //PLCCore::S_IO->toggleRelay(BlowGunPin, true);
    //Main_TM->scheduleOnce(1500, BlowGunOff, TIME_MILLIS);
    IO->toggleRelay(BlowGunPin, true);
    delay(DB->SCR11_GETSET_BlowGunCalibMS.getValue() * 100 / 500 * DB->SCR3_BlowGunVolume.getValue() * 250);
    IO->toggleRelay(BlowGunPin, false);
}

bool PLCCore::commMixerRotate(bool run)
{
    if ((Current_TaskInProccess == 1 && Current_TaskFinished == 1) || Current_TaskInProccess == 0)
    {
        Current_MixingTask = run;
        return true;
    }
    else
    {
        Current_MixingTask = false;
        return false;
    }
}

bool PLCCore::commWaterPump(bool run)
{
    if (Current_TaskInProccess == 0)
    {
        Current_WashingTask = run;
        return true;
    }
    else
    {
        Current_WashingTask = false;
        return false;
    }
}

bool PLCCore::callPasteurization(uint8_t estMin)
{
    if (Current_TaskInProccess == 1 || Current_WashingTask || Current_MixingTask)
        return false;
    else
    {
        Current_EstimatedTimeMM = estMin <= 0 ? DB->SCR10_GETSET_PasteurDuratMM.getValue() : estMin;
        Current_EstimatedHeatMM = DB->SCR10_GETSET_HeatingDuratMM.getValue();
        Current_TaskProccessStep = 2;
        Global::EE.writeEEPROM(taskStepEstimatedMin_EEAddr, Current_EstimatedTimeMM);
        Global::EE.writeEEPROM(isTaskInProccessBit_EEAddr, Current_TaskInProccess = 1);
        Current_TaskFinished = 0;
        Current_HeatFinished = Current_EstimatedHeatMM == 0 ? 1 : 0;
        this->progDispatcher();
    }
    return true;
}

void PLCCore::progDispatcher()
{
    //if (Current_TaskInProccess == 0)
        //return - 1;
    uint8_t Step = Current_TaskProccessStep;
    Current_TaskProccessStep = 2;

    if (Current_TaskInProccess == 1 && Current_TaskFinished == 0)
        progPasteurization();
    else if (Current_TaskInProccess == 1 && Current_TaskFinished == 1 && Current_HeatFinished == 0)
        progHeating();

    if (Current_TaskInProccess == 1 && Current_TaskFinished == 1 && Current_HeatFinished == 1)
    {
        Current_TaskInProccess = 0;
        Current_TaskFinished = 0;
        Current_HeatFinished = 0;
    }

    if (Current_TaskInProccess == 0)
    {
        IO->toggleRelay(Y01, Current_MixingTask);
        IO->toggleRelay(Y03, Current_WashingTask);

        if (Current_MixingTask && Current_WashingTask)
            Current_TaskProccessStep = 12;
        else if (Current_MixingTask)
            Current_TaskProccessStep = 11;
        else if (Current_WashingTask)
            Current_TaskProccessStep = 13;
    }

    if (Step != Current_TaskProccessStep)
        DB->SCR1_TaskProgress_SCR1n2_Description.setAndWriteToOP(Current_TaskProccessStep);
}

uint8_t PLCCore::progPasteurization()
{
    bool isPasteurTempC_Reached = false;
    Current_TaskProccessStep = 4;

    if (Current_EstimatedTimeMM == 0)
    {
        Current_TaskProccessStep = 8;
        Current_TaskFinished = 1;
        return Current_TaskProccessStep;
    }

    if (IO->readDigitalInput(X00))
        Current_TaskProccessStep = 5;
    else return Current_TaskProccessStep;

    if (IO->readDigitalInput(X01))
        Current_TaskProccessStep = 6;
    else return Current_TaskProccessStep;

    if (Current_TaskProccessStep == 6)
        isPasteurTempC_Reached = taskPIDheater(DB->SCR10_GETSET_PasteurTempC.getValue(), true);
    
    if (isPasteurTempC_Reached)
        Current_TaskProccessStep = 7;

    return Current_TaskProccessStep;
}

uint8_t PLCCore::progHeating()
{

    Current_TaskProccessStep = 8;

    if (taskPIDheater(DB->SCR10_GETSET_HeatingTempC.getValue(), Current_MixingTask))
        Current_TaskProccessStep = Current_MixingTask ? 10 : 9;

    return Current_TaskProccessStep;
}

bool PLCCore::taskPIDheater(uint8_t expTempC, bool Mixing)
{
    if (expTempC <= 0)
    {
        IO->toggleRelay(Y02, false);
        return true;
    }

    uint32_t CalibTempC = 2;
    uint32_t Milk_TempC = IO->readAnalogInput(AD0); /* CALIBRATION HERE */
    if (Milk_TempC == -1)
    {
        IO->toggleRelay(Y01, false);
        IO->toggleRelay(Y02, false);
        return false;
    }

    IO->toggleRelay(Y01, Mixing);
    if (Milk_TempC < expTempC)
        IO->toggleRelay(Y02, true);
    else
        IO->toggleRelay(Y02, false);

    return Milk_TempC > expTempC - CalibTempC && Milk_TempC < expTempC + CalibTempC ? true : false;
}

void PLCCore::emergencyStop()
{
    this->Current_MixingTask = false;
    this->Current_WashingTask = false;
    this->taskPIDheater(0, false);
    this->Current_TaskInProccess = 0;
    this->Current_TaskFinished = 0;
    this->Current_HeatFinished = 0;
}

void PLCCore::checkWriteCyclePossibility(bool bootingUp)
{
  bool isOnBootLimitReached = false;
  if (bootingUp)
    Global::EE_currSectCyclCnt = 0;

  uint16_t Depth = -1; 
  uint8_t highByteValue = 0;
  uint8_t lowByteValue = 0;
  uint16_t OLDEE_currSectCyclCnt = 0;

  do
  {
    ++Depth;
    Global::EE.readEEPROM(Global::EE_startAddr + Global::EE_cyclCnt_HB_Addr + Global::EE_sectorSize * Depth, &highByteValue);
    Global::EE.readEEPROM(Global::EE_startAddr + Global::EE_cyclCnt_LB_Addr + Global::EE_sectorSize * Depth, &lowByteValue);
    OLDEE_currSectCyclCnt = Global::EE_currSectCyclCnt;
    Global::EE_currSectCyclCnt = (highByteValue << 8) + lowByteValue;

    if (Global::PLCCore_SerialLog)
    {
        Serial.println("");
        Serial.print("*0x");
        Serial.print(Global::EE_startAddr);
        Serial.print(" loop. depth = ");
        Serial.print(Depth);
        Serial.print(". old(current) cycles = ");
        Serial.print(Global::EE_currSectCyclCnt);
        Serial.println("*");
    }
  }
  while (Global::EE_currSectCyclCnt >= Global::EE_maxWriteCycle);

  if (OLDEE_currSectCyclCnt >= Global::EE_maxWriteCycle && bootingUp && Global::EE_currSectCyclCnt == 0)
        bootingUp = false;

  if (Depth > 0)
  {
    if (Global::PLCCore_SerialLog)
    {
        Serial.print("*eeprom sector changed from 0x");
        Serial.print(Global::EE_startAddr);
        Serial.print(" to 0x");
    }

    Global::EE_startAddr = Global::EE_startAddr + Global::EE_sectorSize * Depth;

    if (Global::PLCCore_SerialLog)
    {
        Serial.print(Global::EE_startAddr);
        Serial.println("*");
    }

    if (!bootingUp)
      readWriteToAnotherSector();
  }
  else if (Global::PLCCore_SerialLog)
    Serial.println("*eeprom sector is valid*");
}

void PLCCore::readWriteToAnotherSector()
{
  bool check1 = false;
  bool check2 = false;
  uint8_t EE_result_local;

  for (uint16_t point = Global::EE_startAddr - Global::EE_sectorSize; point < Global::EE_startAddr; point++)
  {
    check1 = Global::EE.readEEPROM(point, &EE_result_local);
    check2 = Global::EE.writeEEPROM(point + Global::EE_sectorSize, EE_result_local);
    
    if ((!check1 || !check2) && !Global::PLCCore_SerialLog)
        break;

    if (check1 && check2 && Global::PLCCore_SerialLog)
    {
      Serial.print("0x");
      Serial.print(point);
      Serial.print(" .. ");
      Serial.println(point + Global::EE_sectorSize);
    }
    else if (Global::PLCCore_SerialLog)
    {
      Serial.print("smth wrong...");
      break;
    }
    
  }
  if (Global::PLCCore_SerialLog)
    Serial.println("");
}

//void BlowGunOff() {
//    PLCCore::S_IO->toggleRelay(BlowGunPin, false);
//}