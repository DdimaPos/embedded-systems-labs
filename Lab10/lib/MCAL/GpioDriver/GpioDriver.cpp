#include "GpioDriver.h"

void GpioDriver_PinMode(uint8_t pin, uint8_t mode)
{
    pinMode(pin, mode);
}

void GpioDriver_Write(uint8_t pin, uint8_t val)
{
    digitalWrite(pin, val);
}

uint8_t GpioDriver_Read(uint8_t pin)
{
    return digitalRead(pin);
}

void GpioDriver_Toggle(uint8_t pin)
{
    digitalWrite(pin, !digitalRead(pin));
}
