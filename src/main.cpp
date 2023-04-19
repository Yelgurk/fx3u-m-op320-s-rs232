#include <Arduino.h>
#include "global.hpp"

/* MODBUS RS232 (PLC -> OP / MASTER -> SLAVE) */
ModbusMaster Global::OP = ModbusMaster();

/* MAIN */
#include "plcCore.hpp"
PLCCore FX3U;
DataBase *PLCCore::S_DB = FX3U.DB;
IOMonitor *PLCCore::S_IO = FX3U.IO;
uint8_t Global::Freeq = 5;
uint8_t Global::EE_result = 0;
uint8_t Global::EE_valueAddr = 0x0000;
uint16_t Global::EE_maxWriteCycle = 60000;
uint16_t Global::EE_startAddr = 0x0000;
uint16_t Global::EE_sectorSize = 0x001E;
uint16_t Global::EE_currSectCyclCnt = 0;
uint8_t Global::EE_cyclCnt_HB_Addr = 0x0018;
uint8_t Global::EE_cyclCnt_LB_Addr = 0x0017;
bool Global::PLCCore_SerialLog = false;

/* TASK MGMT */
/* include's in plcCore.hpp */
BasicArduinoInterruptAbstraction interruptAbstractionOnly;

/* EEPROM */
ee24c64 Global::EE = ee24c64();

/* RTC */
STM32RTC& rtc = STM32RTC::getInstance();
static uint32_t atime = 1000;
void alarmMatch(void *data);

int taskID_loopState = 0;
int taskID_commRead = 0;
int taskID_eeCycles = 0;
int taskID_progDisp = 0;
void taskBridge_loopState() { FX3U.taskUpdateOPInfo(); }
void taskBridge_commRead() { FX3U.microReadCommands(); }
void taskBridge_eeCycles() { FX3U.checkWriteCyclePossibility(false); }
void taskBridge_progDisp() { FX3U.progDispatcher(); }

void setup()
{
  /* Serial, 38400 baud, for 232 as modbus master -> OP slave */
  Serial.setRx(PA10);
  Serial.setTx(PA9);
  Serial.begin(38400);

  /* modbus begin, slave ID = 1  +  eeprom init, DB init (global)*/
  Global::OP.begin(1, Serial);
  Global::EE.init();

  /* rtc alarm init */
  rtc.attachInterrupt(alarmMatch, &atime);
  rtc.enableAlarm(rtc.MATCH_HHMMSS);
  rtc.begin();
  
  delay(100);
  FX3U.checkWriteCyclePossibility(true);
  delay(100);
  FX3U.init();
  FX3U.initRTC();
  FX3U.initAlarm();
  FX3U.initConfig();
  FX3U.initState();
  FX3U.bootInfo();

  FX3U.Main_TM = &taskManager;
  taskID_loopState = taskManager.scheduleFixedRate(5, taskBridge_loopState, TIME_SECONDS);
  taskID_commRead = taskManager.scheduleFixedRate(20, taskBridge_commRead, TIME_MILLIS);
  taskID_eeCycles = taskManager.scheduleFixedRate(60, taskBridge_eeCycles, TIME_SECONDS);
  taskID_progDisp = taskManager.scheduleFixedRate(500, taskBridge_progDisp, TIME_MILLIS);
  delay(50);
}

void loop() {
  taskManager.runLoop();
}

void alarmMatch(void *data)
{
    FX3U.emergencyStop();
}