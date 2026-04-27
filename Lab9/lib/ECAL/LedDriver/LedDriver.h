#pragma once
#include <stdint.h>

#define LED_PIN             25   /* Single actuator LED                      */
#define LED_LEDC_CHANNEL     0   /* LEDC channel 0 (free after MotorDriver   */
                                 /*   deletion which previously claimed it)  */
#define LED_LEDC_FREQ     1000   /* 1 kHz PWM carrier frequency              */
#define LED_LEDC_RES         8   /* 8-bit duty resolution: 0 – 255           */

void    LedDriver_Init(void);
void    LedDriver_On(void);               /* Full brightness  (duty = 255)   */
void    LedDriver_Off(void);              /* Off              (duty = 0)     */
void    LedDriver_SetDuty(uint8_t duty);  /* Arbitrary 0-255  (for PID use)  */
uint8_t LedDriver_GetDuty(void);
