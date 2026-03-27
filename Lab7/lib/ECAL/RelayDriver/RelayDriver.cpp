#include "RelayDriver.h"
#include "GpioDriver.h"

static uint8_t relayState = 0;

void RelayDriver_Init(void)
{
    GpioDriver_PinMode(RELAY_PIN, OUTPUT);
    GpioDriver_Write(RELAY_PIN, LOW);
    relayState = 0;
}

void RelayDriver_On(void)
{
    GpioDriver_Write(RELAY_PIN, HIGH);
    relayState = 1;
}

void RelayDriver_Off(void)
{
    GpioDriver_Write(RELAY_PIN, LOW);
    relayState = 0;
}

void RelayDriver_Toggle(void)
{
    if (relayState) {
        RelayDriver_Off();
    } else {
        RelayDriver_On();
    }
}

uint8_t RelayDriver_GetState(void)
{
    return relayState;
}
