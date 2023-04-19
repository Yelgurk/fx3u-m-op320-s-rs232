#include "Arduino.h"
#include "SlowSoftI2CMaster.h"

#define EEI2Caddr 0x50

class ee24c64
{
public:
    SlowSoftI2CMaster *si;

public:
    ee24c64();
    bool init();
    bool readEEPROM(unsigned long address, uint8_t *byte);
    bool writeEEPROM(long unsigned address, uint8_t byte);
    bool deleteEEPROM(long unsigned from, unsigned long to, uint8_t byte, boolean poll);
};