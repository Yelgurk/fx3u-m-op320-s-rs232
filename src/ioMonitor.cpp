#include "ioMonitor.hpp"

IOMonitor::IOMonitor() {}

void IOMonitor::init()
{
    byte i = 0;
    for (; i < sizeof(DigIn_pinMap) / sizeof(uint8_t); i++)
        pinMode(DigIn_pinMap[i], INPUT);
    delay(50);
    
    for (i = 0;  i < sizeof(ROut_pinMap) / sizeof(uint8_t); i++)
        pinMode(ROut_pinMap[i], OUTPUT);
    delay(50);
}

uint32_t IOMonitor::readAnalogInput(byte plcInputID)
{
    if (plcInputID < 0 || plcInputID >= sizeof(AnIn_pinMap) / sizeof(uint8_t))
        return -1;
    else
        AnIn_value[plcInputID] = analogRead(AnIn_pinMap[plcInputID]);
    return AnIn_value[plcInputID];
}

bool IOMonitor::readDigitalInput(byte plcInputID)
{
    if (plcInputID < 0 || plcInputID >= sizeof(DigIn_pinMap) / sizeof(uint8_t))
        return false;
    else
        DigIn_value[plcInputID] = digitalRead(DigIn_pinMap[plcInputID]);
    return DigIn_value[plcInputID] == HIGH ? true : false;
}

void IOMonitor::readAllInputs()
{
    byte i = 0;
    for (; i < sizeof(DigIn_pinMap) / sizeof(uint8_t); i++)
        DigIn_value[i] = digitalRead(DigIn_pinMap[i]);

    for (i = 0;  i < sizeof(AnIn_pinMap) / sizeof(uint8_t); i++)
        AnIn_value[i] = analogRead(AnIn_pinMap[i]);
}

bool IOMonitor::toggleRelay(byte plcRelayID, bool state)
{
    if (plcRelayID < 0 || plcRelayID >= sizeof(ROut_pinMap) / sizeof(uint8_t))
        return false;
    else
    {
        ROut_value[plcRelayID] = state;
        digitalWrite(ROut_pinMap[plcRelayID], state ? HIGH : LOW);
        return true;
    }
}

void IOMonitor::offAllRelays()
{
    for (byte i = 0; i < sizeof(ROut_pinMap) / sizeof(uint8_t); i++)
    {
        ROut_value[i] = false;
        digitalWrite(ROut_pinMap[i], LOW);
    }
}