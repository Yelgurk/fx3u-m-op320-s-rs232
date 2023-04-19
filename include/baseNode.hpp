#include "Arduino.h"
#include "global.hpp"

class BaseNode
{
private:
    long OPaddr = 0x0000;
    long EEaddr = 0x0000;
    bool isOPCommand = false;
    bool isSavedInEE = false;
    uint8_t value = 0;
    byte OPvalueScale = 1;
    byte OverrideValue_Temp = 0;
    uint8_t highByteValue = 0;
    uint8_t lowByteValue = 0;

public:
    BaseNode();
    bool eepromWriteCycleControl(bool writeExecuted);
    void init(long OPaddr);
    void init(long OPaddr, bool isSavedInEE, long EEaddr);
    long getOPAddr();
    void setScale(byte scale);
    void setValue(byte value);
    void setValue(bool value);
    void setAndWriteToEE(byte value);
    void setAndWriteToOP(byte value);
    byte getValue();
    bool writeToEE();
    bool readFromEE();
    void writeToOP();
    uint8_t readFromOP();
    void fromEEtoOP();
    void fromOPtoEE();
};