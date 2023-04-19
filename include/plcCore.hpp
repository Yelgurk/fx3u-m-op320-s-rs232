#pragma once
#include "Arduino.h"
#include <STM32RTC.h>
#include <TaskManagerIO.h>
#include <BasicInterruptAbstraction.h>
#include "dataBase.hpp"
#include "ioMonitor.hpp"

/* OUT (Relay - load control) */
#define Y00 0 //blowgun reserved
#define Y01 1 //test: mixer driver
#define Y02 2 //test: heater driver
#define Y03 3 //test: water pump driver
#define Y04 4
#define Y05 5

/* Digital IN */
#define X00 0 //test: water jacket (fill or not == true/false)
#define X01 1 //test: milk in tanker (fill or not == true/false)
#define X02 2
#define X03 3
#define X04 4
#define X05 5
#define X06 6
#define X07 7

/* Analog IN */
#define AD0 0 //test: milk temp (0..60C* == 0..255)
#define AD1 1 
#define AD2 2
#define AD3 3
#define AD4 4
#define AD5 5

/* Bindings to physical in/out pins */
#define BlowGunPin Y00

//void BlowGunOff();

class PLCCore
{
public:
    DataBase *DB;
    IOMonitor *IO;
    TaskManager *Main_TM;
    static DataBase *S_DB;
    static IOMonitor *S_IO;

private:
    STM32RTC& rtc = STM32RTC::getInstance();

    const long isTimeSetBit_EEAddr = 0x001D;
    const long isAlarmSetBit_EEAddr = 0x001C;
    const long isTaskInProccessBit_EEAddr = 0x001B;
    //const long taskProccessStep_EEAddr = 0x001A;
    const long taskStepEstimatedMin_EEAddr = 0x001A;
    byte EEPROMresult = 0;

    uint8_t Current_TaskInProccess = 0;
    uint8_t Current_TaskFinished = 0;
    uint8_t Current_HeatFinished = 0;
    uint8_t Current_AlarmIsSetted = 0;
    uint8_t Current_TaskProccessStep = 0;
    uint8_t Current_EstimatedTimeMM = 0;
    uint8_t Current_EstimatedHeatMM = 0;
    uint8_t Current_ProgStartRTCMM = 0;
    bool Current_WashingTask = false;
    bool Current_MixingTask = false;

    uint8_t Command_Handler[13];
    bool Command_Handled = false;

    uint8_t Control_BlowGunPump_Pin = Y00;
    uint8_t Control_Mixer_Pin = Y01;
    uint8_t Control_Heater_Pin = Y02;
    uint8_t Control_WaterPump_Pin = Y03;
    uint8_t Sensor_ProducTempC_Pin = AD0;
    uint8_t Sensor_WaterJacketTempC_Pin = AD1;
    uint8_t Sensor_BatteryCharge_Pin = AD2;

    bool bootOP320PasteurError = true;

public:
    PLCCore();
    void init();
    void setNewRTC();
    void bootInfo();
    void checkEepromCycle();
    void microReadCommands();
    void taskUpdateOPInfo();
    void taskUpdateOPConfig();
    bool commMixerRotate(bool run);
    bool commWaterPump(bool run);
    void taskBlowGunDosage();
    bool taskPIDheater(uint8_t expTempC, bool Mixing = false);
    bool callPasteurization(uint8_t estMin);
    void progDispatcher();
    uint8_t progPasteurization();
    uint8_t progHeating();
    void emergencyStop();
    void checkWriteCyclePossibility(bool bootingUp = false);
    void readWriteToAnotherSector();

public:
    void initRTC();
    void initAlarm();
    void initConfig();
    void initState();
};