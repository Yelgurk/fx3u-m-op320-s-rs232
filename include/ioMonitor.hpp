#include "Arduino.h"

class IOMonitor
{
private:
    const uint8_t RUN_LED_PIN = PD10;
    const uint8_t POTENTIOMETER1_PIN = PC2;
    const uint8_t POTENTIOMETER2_PIN = PC3;
    const uint8_t DAC0_PIN = PA4;
    const uint8_t DAC1_PIN = PA5;
    const uint8_t AnIn_pinMap[6] = { PA1, PA3, PC4, PC5, PC0, PC1 };
    const uint8_t DigIn_pinMap[8] = { PB13, PB14, PB11, PB12, PE15, PB10, PE13, PE14 };
    const uint8_t ROut_pinMap[6] = { PC9, PC8, PA8, PA0, PB3, PD12 };

public:
    uint32_t AnIn_value[6] = { 0 };
    bool DigIn_value[8] = { 0 };
    bool ROut_value[6] = { 0 };

    IOMonitor();
    void init();
    uint32_t readAnalogInput(byte plcInputID);
    bool readDigitalInput(byte plcInputID);
    void readAllInputs();
    bool toggleRelay(byte plcRelayID, bool state);
    void offAllRelays();
};