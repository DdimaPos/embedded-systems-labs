#pragma once
#include <stdint.h>

#define MOTOR_IN1_PIN  16 
#define MOTOR_IN2_PIN  17
#define MOTOR_ENA_PIN  18   /* PWM speed (LEDC) */

#define MOTOR_LEDC_CHANNEL  0
#define MOTOR_LEDC_FREQ     5000   /* 5 kHz */
#define MOTOR_LEDC_RES      8      /* 8-bit: 0-255 */

void    MotorDriver_Init(void);
void    MotorDriver_SetSpeed(uint8_t duty);   /* 0-255 */
void    MotorDriver_SetDirection(uint8_t dir); /* 0=forward, 1=reverse */
void    MotorDriver_Stop(void);
uint8_t MotorDriver_GetDuty(void);
