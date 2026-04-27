/*
 * TaskControl – ON/OFF with hysteresis for light-level control.
 *
 * Period justification: 200 ms is fast enough to react to light changes while
 * keeping switching events infrequent. The hysteresis dead band prevents LED
 * chattering at small light fluctuations around the setpoint.
 *
 * Hysteresis logic:
 *   lower = SP − HY/2
 *   upper = SP + HY/2
 *
 *   light < lower  →  LED ON    (environment too dark, add light)
 *   light > upper  →  LED OFF   (bright enough, extinguish)
 *   otherwise      →  hold current state (dead band, prevent chatter)
 *
 * Fault safety: if TaskAcquisition sets the fault flag the LED is forced OFF.
 */
#include "TaskControl.h"
#include "SystemData.h"
#include "LedDriver.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void TaskControl_Task(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        float light    = SystemData_GetLightLevel();
        float sp       = SystemData_GetSetpoint();
        float hy       = SystemData_GetHysteresis();
        int   ledState = SystemData_GetLedState();
        int   fault    = SystemData_GetFault();

        if (fault) {
            /* Safe state: LED off */
            LedDriver_Off();
            SystemData_SetLedState(0);
            vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CONTROL_PERIOD_MS));
            continue;
        }

        float lower = sp - hy * 0.5f;
        float upper = sp + hy * 0.5f;

        int newState = ledState;   /* default: hold current state (dead band) */

        if (light < lower) newState = 1;   /* too dark  → LED ON  */
        if (light > upper) newState = 0;   /* too bright → LED OFF */

        /* Apply to hardware only on state change to avoid redundant LEDC writes */
        if (newState != ledState) {
            if (newState == 1) LedDriver_On();
            else               LedDriver_Off();
            SystemData_SetLedState(newState);
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CONTROL_PERIOD_MS));
    }
}
