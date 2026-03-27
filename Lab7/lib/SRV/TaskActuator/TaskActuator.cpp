#include "TaskActuator.h"
#include "ActuatorData.h"
#include "MotorDriver.h"
#include "RelayDriver.h"

void TaskActuator_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        /* ── Analog actuator (motor/LED PWM) ── */
        float currentSpeed = ActuatorData_GetCurrentSpeed();
        Direction_t dir    = ActuatorData_GetDirection();

        /* Map 0-100% to 0-255 duty cycle */
        uint8_t duty = (uint8_t)((currentSpeed * 255.0f) / 100.0f);

        if (currentSpeed < 1.0f) {
            MotorDriver_Stop();
        } else {
            MotorDriver_SetDirection((uint8_t)dir);
            MotorDriver_SetSpeed(duty);
        }

        /* ── Binary actuator (relay) ── */
        RelayState_t relayTarget = ActuatorData_GetRelayTarget();
        RelayState_t relayCurrent = ActuatorData_GetRelayState();

        if (relayTarget != relayCurrent) {
            if (relayTarget == RELAY_ON) {
                RelayDriver_On();
            } else {
                RelayDriver_Off();
            }
            ActuatorData_SetRelayState(relayTarget);
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ACTUATOR_PERIOD_MS));
    }
}
