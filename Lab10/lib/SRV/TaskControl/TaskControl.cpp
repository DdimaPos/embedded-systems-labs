/*
 * TaskControl – ON/OFF with hysteresis  OR  PID, selectable at runtime.
 *
 * Period: CONTROL_PERIOD_MS = 200 ms → dt = 0.200 s for PID integration.
 *
 * ON-OFF mode:
 *   light < SP − HY/2  →  LED ON    (too dark)
 *   light > SP + HY/2  →  LED OFF   (bright enough)
 *   otherwise          →  hold state (dead band, prevents chattering)
 *
 * PID mode:
 *   error  = setpoint − light
 *   output = Kp·error  +  Ki·∫error·dt  −  Kd·Δmeasurement/dt
 *   output is clamped to [0, 100 %] and mapped to LED duty [0, 255].
 *   Derivative is computed on measurement (not error) to avoid derivative
 *   kick when the setpoint is changed at runtime.
 *
 * Mode transitions: PID state (integral, prevMeasurement) is reset whenever
 * the active mode changes, preventing stale integral windup.
 *
 * Fault safety: LED forced OFF in both modes when the fault flag is set.
 */
#include "TaskControl.h"
#include "SystemData.h"
#include "LedDriver.h"
#include "PidController.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#define CONTROL_DT  (CONTROL_PERIOD_MS / 1000.0f)   /* seconds */

void TaskControl_Task(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    /* PID instance lives inside the task — only TaskControl touches it */
    PidController_t pid;
    Pid_Init(&pid,
             DEFAULT_KP, DEFAULT_KI, DEFAULT_KD,
             0.0f, 100.0f,    /* output range: % */
             CONTROL_DT);

    ControlMode_t prevMode = CTRL_MODE_ONOFF;

    for (;;) {
        float         light    = SystemData_GetLightLevel();
        float         sp       = SystemData_GetSetpoint();
        float         hy       = SystemData_GetHysteresis();
        int           ledState = SystemData_GetLedState();
        int           fault    = SystemData_GetFault();
        ControlMode_t mode     = SystemData_GetControlMode();

        /* ── Fault safety ────────────────────────────────────────────────── */
        if (fault) {
            LedDriver_Off();
            SystemData_SetLedState(0);
            SystemData_SetPidOutput(0.0f);
            vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CONTROL_PERIOD_MS));
            continue;
        }

        /* ── Mode transition: reset PID state to avoid windup ───────────── */
        if (mode != prevMode) {
            Pid_Reset(&pid);
            prevMode = mode;
        }

        /* ── Sync PID tunings from SystemData (updated by TaskCommand) ───── */
        if (mode == CTRL_MODE_PID) {
            Pid_SetTunings(&pid,
                           SystemData_GetKp(),
                           SystemData_GetKi(),
                           SystemData_GetKd());
        }

        /* ── Control law ─────────────────────────────────────────────────── */
        if (mode == CTRL_MODE_PID) {

            float outputPct = Pid_Compute(&pid, sp, light);
            /* Map 0-100 % to 0-255 duty cycle */
            uint8_t duty = (uint8_t)(outputPct * 2.55f);
            LedDriver_SetDuty(duty);
            /* led state: treat > 0 duty as ON for binary reporting */
            SystemData_SetLedState(duty > 0 ? 1 : 0);
            SystemData_SetPidOutput(outputPct);

        } else {   /* CTRL_MODE_ONOFF */

            float lower   = sp - hy * 0.5f;
            float upper   = sp + hy * 0.5f;
            int   newState = ledState;   /* default: hold (dead band) */

            if (light < lower) newState = 1;
            if (light > upper) newState = 0;

            if (newState != ledState) {
                if (newState == 1) LedDriver_On();
                else               LedDriver_Off();
                SystemData_SetLedState(newState);
            }
            /* Output % for reporter: 100 when ON, 0 when OFF */
            SystemData_SetPidOutput(newState ? 100.0f : 0.0f);
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CONTROL_PERIOD_MS));
    }
}
