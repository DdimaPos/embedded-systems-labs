#include "MotorDriver.h"
#include "GpioDriver.h"
#include <Arduino.h>

static uint8_t currentDuty = 0;

void MotorDriver_Init(void)
{
    GpioDriver_PinMode(MOTOR_IN1_PIN, OUTPUT);
    GpioDriver_PinMode(MOTOR_IN2_PIN, OUTPUT);
    GpioDriver_Write(MOTOR_IN1_PIN, LOW);
    GpioDriver_Write(MOTOR_IN2_PIN, LOW);

    ledcSetup(MOTOR_LEDC_CHANNEL, MOTOR_LEDC_FREQ, MOTOR_LEDC_RES);
    ledcAttachPin(MOTOR_ENA_PIN, MOTOR_LEDC_CHANNEL);
    ledcWrite(MOTOR_LEDC_CHANNEL, 0);
    currentDuty = 0;
}

void MotorDriver_SetSpeed(uint8_t duty)
{
    currentDuty = duty;
    ledcWrite(MOTOR_LEDC_CHANNEL, duty);
}

void MotorDriver_SetDirection(uint8_t dir)
{
    if (dir == 0) {
        /* Forward: IN1=HIGH, IN2=LOW */
        GpioDriver_Write(MOTOR_IN1_PIN, HIGH);
        GpioDriver_Write(MOTOR_IN2_PIN, LOW);
    } else {
        /* Reverse: IN1=LOW, IN2=HIGH */
        GpioDriver_Write(MOTOR_IN1_PIN, LOW);
        GpioDriver_Write(MOTOR_IN2_PIN, HIGH);
    }
}

void MotorDriver_Stop(void)
{
    currentDuty = 0;
    ledcWrite(MOTOR_LEDC_CHANNEL, 0);
    GpioDriver_Write(MOTOR_IN1_PIN, LOW);
    GpioDriver_Write(MOTOR_IN2_PIN, LOW);
}

uint8_t MotorDriver_GetDuty(void)
{
    return currentDuty;
}
