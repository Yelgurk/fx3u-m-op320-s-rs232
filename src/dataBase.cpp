#include "dataBase.hpp"

DataBase::DataBase()
{
    GLOBAl_OPScreen.init(0x0000, false, 0x0000);

    SCR1_BatteryState.init(0x0001, false, 0x0000);
    SCR1_MilkTempC.init(0x0002, false, 0x0000);
    SCR1_TasksCount.init(0x0003, false, 0x0000);
    SCR1_TaskDescription.init(0x0004, false, 0x0000);
    SCR1_TaskProgress_SCR1n2_Description.init(0x0005, false, 0x0000);
    SCR1_TaskStepByStepDescription.init(0x0006, false, 0x0000);

    SCR3_BlowGunVolume.init(0x0007, true, 0x000E);
    SCR6_GETSET_Day.init(0x0008, true, 0x0000);
    SCR6_GETSET_Month.init(0x0009, true, 0x0001);
    SCR6_GETSET_Year.init(0x000A, true, 0x0002);
    SCR6_GETSET_RTCHH.init(0x000B, true, 0x0003);
    SCR6_GETSET_RTCMM.init(0x000C, true, 0x0004);
    SCR6_GETSET_AlarmHH.init(0x000D, true, 0x0005);
    SCR6_GETSET_AlarmMM.init(0x000E, true, 0x0006);
    SCR6_AlarmMode.init(0x000F, true, 0x0007);
    SCR9_GETSET_LCDfreeq.init(0x0010, true, 0x0008);
    SCR10_GETSET_PasteurDuratMM.init(0x0011, true, 0x0009);
    SCR10_GETSET_HeatingDuratMM.init(0x0012, true, 0x000A);
    SCR10_GETSET_PasteurTempC.init(0x0013, true, 0x000B);
    SCR10_GETSET_HeatingTempC.init(0x0014, true, 0x000C);
    SCR11_GETSET_BlowGunCalibMS.init(0x0015, true, 0x000D);

    SCR1_COMM_ESC_ToCancelScreen.init(0x0064);
    SCR2_COMM_9_AcceptCancelling.init(0x0065);
    SCR3_COMM_UP_BlowgunPlus250.init(0x0066);
    SCR3_COMM_DOWN_BlowgunMin250.init(0x0067);
    SCR3_COMM_FreeButt_StartBlowgun.init(0x0068);
    SCR6_COMM_UPDOWN_SwitchAlarmMode.init(0x0069);
    SCR6_COMM_SET_AcceptNewOPDateTime.init(0x006A);
    SCR9_COMM_SET_AcceptNewDataFreeq.init(0x006B);
    SCR10_COMM_SET_AcceptNewPHParams.init(0x006C);
    SCR11_COMM_SET_AcceptBlowgunCalib.init(0x006D);
    SCR11_COMM_TestBlowgunCalib.init(0x006E);
    SCR1_COMM_ALM_ONOFFMixer.init(0x006F);
    SCR4_COMM_2_StartWashingProgramm.init(0x0070);
    SCR4_COMM_1_StartPasteurProgramm.init(0x0071);

    SCR3_BlowGunVolume.setScale(250);
}

void DataBase::init()
{
    SCR3_BlowGunVolume.readFromEE();
    SCR6_GETSET_Day.readFromEE();
    SCR6_GETSET_Month.readFromEE();
    SCR6_GETSET_Year.readFromEE();
    SCR6_GETSET_RTCHH.readFromEE();
    SCR6_GETSET_RTCMM.readFromEE();
    SCR6_GETSET_AlarmHH.readFromEE();
    SCR6_GETSET_AlarmMM.readFromEE();
    SCR6_AlarmMode.readFromEE();
    SCR9_GETSET_LCDfreeq.readFromEE();
    SCR10_GETSET_PasteurDuratMM.readFromEE();
    SCR10_GETSET_HeatingDuratMM.readFromEE();
    SCR10_GETSET_PasteurTempC.readFromEE();
    SCR10_GETSET_HeatingTempC.readFromEE();
    SCR11_GETSET_BlowGunCalibMS.readFromEE();
}