/*
 * TaskCommand – serial interface for runtime parameter adjustment.
 *
 * Uses SerialIO_ReadChar() / printf() exclusively (no Serial.* calls).
 *
 * Supported commands (case-sensitive, terminated by '\n'):
 *
 *   ON-OFF control:
 *     SP<val>   Set setpoint (% light).     Example: SP60.0
 *     HY<val>   Set hysteresis band (%).    Example: HY5.0
 *
 *   PID control:
 *     KP<val>   Set proportional gain.      Example: KP3.0
 *     KI<val>   Set integral gain.          Example: KI0.5
 *     KD<val>   Set derivative gain.        Example: KD0.2
 *
 *   Mode selection:
 *     MD0       Switch to ON-OFF mode
 *     MD1       Switch to PID mode
 *
 *   Status:
 *     ?         Print full system status
 */
#include "TaskCommand.h"
#include "SystemData.h"
#include "SerialIO.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD_BUF_SIZE 32

static void printStatus(void);

void TaskCommand_Task(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    char buf[CMD_BUF_SIZE];
    int  idx = 0;

    printf("[CMD] Ready.\n");
    printf("[CMD] ON-OFF: SP<pct> HY<pct>\n");
    printf("[CMD] PID   : KP<v> KI<v> KD<v>\n");
    printf("[CMD] Mode  : MD0 (ON-OFF)  MD1 (PID)\n");
    printf("[CMD] Status: ?\n");

    for (;;) {
        int c;
        while ((c = SerialIO_ReadChar()) != -1) {
            if (c == '\r') continue;

            if (c == '\n' || idx >= CMD_BUF_SIZE - 1) {
                buf[idx] = '\0';
                idx = 0;

                if (buf[0] == '\0') continue;

                /* ── Setpoint ──────────────────────────────────────────── */
                if (buf[0] == 'S' && buf[1] == 'P') {
                    SystemData_SetSetpoint((float)atof(&buf[2]));
                    printf("[CMD] SP -> %.1f %%\n", SystemData_GetSetpoint());

                /* ── Hysteresis ────────────────────────────────────────── */
                } else if (buf[0] == 'H' && buf[1] == 'Y') {
                    SystemData_SetHysteresis((float)atof(&buf[2]));
                    float hy = SystemData_GetHysteresis();
                    float sp = SystemData_GetSetpoint();
                    printf("[CMD] HY -> %.1f %%  band [%.1f .. %.1f]\n",
                           hy, sp - hy * 0.5f, sp + hy * 0.5f);

                /* ── PID gains ─────────────────────────────────────────── */
                } else if (buf[0] == 'K' && buf[1] == 'P') {
                    SystemData_SetKp((float)atof(&buf[2]));
                    printf("[CMD] Kp -> %.3f\n", SystemData_GetKp());

                } else if (buf[0] == 'K' && buf[1] == 'I') {
                    SystemData_SetKi((float)atof(&buf[2]));
                    printf("[CMD] Ki -> %.3f\n", SystemData_GetKi());

                } else if (buf[0] == 'K' && buf[1] == 'D') {
                    SystemData_SetKd((float)atof(&buf[2]));
                    printf("[CMD] Kd -> %.3f\n", SystemData_GetKd());

                /* ── Mode switch ───────────────────────────────────────── */
                } else if (buf[0] == 'M' && buf[1] == 'D') {
                    int m = atoi(&buf[2]);
                    if (m == 0) {
                        SystemData_SetControlMode(CTRL_MODE_ONOFF);
                        printf("[CMD] Mode -> ON-OFF (hysteresis)\n");
                    } else if (m == 1) {
                        SystemData_SetControlMode(CTRL_MODE_PID);
                        printf("[CMD] Mode -> PID  (Kp=%.2f Ki=%.2f Kd=%.2f)\n",
                               SystemData_GetKp(),
                               SystemData_GetKi(),
                               SystemData_GetKd());
                    } else {
                        printf("[CMD] MD: use MD0 (ON-OFF) or MD1 (PID)\n");
                    }

                /* ── Status ────────────────────────────────────────────── */
                } else if (buf[0] == '?') {
                    printStatus();

                } else {
                    printf("[CMD] Unknown: \"%s\"\n", buf);
                }

            } else {
                buf[idx++] = (char)c;
            }
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(COMMAND_PERIOD_MS));
    }
}

static void printStatus(void)
{
    float         light  = SystemData_GetLightLevel();
    float         sp     = SystemData_GetSetpoint();
    float         hy     = SystemData_GetHysteresis();
    ControlMode_t mode   = SystemData_GetControlMode();
    int           led    = SystemData_GetLedState();
    float         outPct = SystemData_GetPidOutput();
    float         kp     = SystemData_GetKp();
    float         ki     = SystemData_GetKi();
    float         kd     = SystemData_GetKd();
    int           fault  = SystemData_GetFault();

    printf("========== STATUS ==========\n");
    printf("  Mode        : %s\n",  mode == CTRL_MODE_PID ? "PID" : "ON-OFF");
    printf("  Light level : %.1f %%\n",  light);
    printf("  Setpoint    : %.1f %%\n",  sp);
    printf("  Output      : %.1f %%\n",  outPct);
    printf("  LED         : %s\n",  led ? "ON" : "OFF");
    if (mode == CTRL_MODE_ONOFF) {
        printf("  Hysteresis  : %.1f %%  [%.1f .. %.1f]\n",
               hy, sp - hy * 0.5f, sp + hy * 0.5f);
    } else {
        printf("  Kp / Ki / Kd: %.3f / %.3f / %.3f\n", kp, ki, kd);
    }
    printf("  Fault       : %s\n",  fault ? "YES" : "no");
    printf("============================\n");
}
