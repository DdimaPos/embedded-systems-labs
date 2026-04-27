#pragma once
#include <Arduino.h>

void    GpioDriver_PinMode(uint8_t pin, uint8_t mode);
void    GpioDriver_Write(uint8_t pin, uint8_t val);
uint8_t GpioDriver_Read(uint8_t pin);
void    GpioDriver_Toggle(uint8_t pin);
