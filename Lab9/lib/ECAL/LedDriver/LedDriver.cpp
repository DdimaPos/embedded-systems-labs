#include "LedDriver.h"
#include <Arduino.h>

static uint8_t s_duty = 0;

void LedDriver_Init(void)
{
    ledcSetup(LED_LEDC_CHANNEL, LED_LEDC_FREQ, LED_LEDC_RES);
    ledcAttachPin(LED_PIN, LED_LEDC_CHANNEL);
    ledcWrite(LED_LEDC_CHANNEL, 0);
    s_duty = 0;
}

void LedDriver_SetDuty(uint8_t duty)
{
    s_duty = duty;
    ledcWrite(LED_LEDC_CHANNEL, duty);
}

void    LedDriver_On(void)      { LedDriver_SetDuty(255); }
void    LedDriver_Off(void)     { LedDriver_SetDuty(0);   }
uint8_t LedDriver_GetDuty(void) { return s_duty;          }
