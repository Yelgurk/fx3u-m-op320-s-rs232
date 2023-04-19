#pragma once
#include "Arduino.h"
#include "ModbusMaster.h"
#include "ee24c64.hpp"

class Global
{
public:
    static ModbusMaster OP;
    static ee24c64 EE;
    static uint8_t Freeq;

    static uint8_t EE_result;
    static uint8_t EE_valueAddr;
    static uint16_t EE_maxWriteCycle;
    static uint16_t EE_startAddr;
    static uint16_t EE_sectorSize;
    static uint16_t EE_currSectCyclCnt;
    static uint8_t EE_cyclCnt_HB_Addr;
    static uint8_t EE_cyclCnt_LB_Addr;
    static bool PLCCore_SerialLog;
};