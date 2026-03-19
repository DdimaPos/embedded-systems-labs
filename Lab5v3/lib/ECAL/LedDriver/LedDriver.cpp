#include "LedDriver.h"
#include "GpioDriver.h"

#define LED_GREEN_PIN  25
#define LED_RED_PIN    26

void LedDriver_Init(void)
{
    GpioDriver_PinMode(LED_GREEN_PIN, OUTPUT);
    GpioDriver_PinMode(LED_RED_PIN,   OUTPUT);
    GpioDriver_Write(LED_GREEN_PIN, LOW);
    GpioDriver_Write(LED_RED_PIN,   LOW);
}

void LedDriver_GreenOn(void)  { GpioDriver_Write(LED_GREEN_PIN, HIGH); }
void LedDriver_GreenOff(void) { GpioDriver_Write(LED_GREEN_PIN, LOW);  }
void LedDriver_RedOn(void)    { GpioDriver_Write(LED_RED_PIN,   HIGH); }
void LedDriver_RedOff(void)   { GpioDriver_Write(LED_RED_PIN,   LOW);  }
