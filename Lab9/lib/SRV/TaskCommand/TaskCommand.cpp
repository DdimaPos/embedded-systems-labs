/*
 * TaskCommand – serial interface for runtime parameter adjustment.
 *
 * Uses SerialIO_ReadChar() / printf() exclusively (no Serial.* calls).
 * SerialIO_ReadChar() returns -1 immediately when no byte is available,
 * so the task polls with vTaskDelayUntil and never blocks the scheduler.
 *
 * Supported commands (case-sensitive, terminated by newline '\n'):
 *   SP<val>   Set setpoint  (% light level).  Example: SP60.0
 *   HY<val>   Set hysteresis band (%).         Example: HY5.0
 *   ?         Print current status
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

    printf("[CMD] Ready. Commands: SP<pct> HY<pct> ?\n");

    for (;;) {
        /* Drain all available characters this tick */
        int c;
        while ((c = SerialIO_ReadChar()) != -1) {
            if (c == '\r') continue;

            if (c == '\n' || idx >= CMD_BUF_SIZE - 1) {
                buf[idx] = '\0';
                idx = 0;

                if (buf[0] == '\0') continue;

                if (buf[0] == 'S' && buf[1] == 'P') {
                    float sp = (float)atof(&buf[2]);
                    SystemData_SetSetpoint(sp);
                    printf("[CMD] Setpoint -> %.1f %%\n", SystemData_GetSetpoint());

                } else if (buf[0] == 'H' && buf[1] == 'Y') {
                    float hy = (float)atof(&buf[2]);
                    SystemData_SetHysteresis(hy);
                    float actual = SystemData_GetHysteresis();
                    float sp     = SystemData_GetSetpoint();
                    printf("[CMD] Hysteresis -> %.1f %%  band [%.1f .. %.1f]\n",
                           actual, sp - actual * 0.5f, sp + actual * 0.5f);

                } else if (buf[0] == '?') {
                    printStatus();

                } else {
                    printf("[CMD] Unknown: \"%s\"  (commands: SP HY ?)\n", buf);
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
    float light = SystemData_GetLightLevel();
    float sp    = SystemData_GetSetpoint();
    float hy    = SystemData_GetHysteresis();
    int   led   = SystemData_GetLedState();
    int   fault = SystemData_GetFault();

    printf("========== STATUS ==========\n");
    printf("  Light level : %.1f %%\n",  light);
    printf("  Setpoint    : %.1f %%\n",  sp);
    printf("  Hysteresis  : %.1f %%  band [%.1f .. %.1f]\n",
           hy, sp - hy * 0.5f, sp + hy * 0.5f);
    printf("  LED         : %s\n",  led   ? "ON"  : "OFF");
    printf("  Fault       : %s\n",  fault ? "YES" : "no");
    printf("============================\n");
}
