/*
 * TaskReporter – display and serial logging.
 *
 * Uses printf() exclusively (no Serial.* calls).
 *
 * Outputs:
 *   1. OLED 128×64 (text size 1 → ~21 chars × 8 rows at 8 px row height)
 *      Layout changes depending on active control mode.
 *   2. Serial Plotter line: >Light:X,SP:X,Output:X  (consistent across modes)
 *   3. Verbose console summary every FULL_REPORT_EVERY_N cycles (~5 s)
 */
#include "TaskReporter.h"
#include "SystemData.h"
#include "OledDisplay.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>

#define FULL_REPORT_EVERY_N  10  /* every 5 s at 500 ms period */

/* ── OLED helpers ────────────────────────────────────────────────────────── */

static void oledOnOff(float light, float sp, float hy, int led, int fault)
{
    char line[32];
    float lower = sp - hy * 0.5f;
    float upper = sp + hy * 0.5f;

    OledDisplay_PrintAt(10, 0,  "ON-OFF LIGHT CTRL");

    snprintf(line, sizeof(line), "Light: %5.1f %%", light);
    OledDisplay_PrintAt(0, 10, line);

    snprintf(line, sizeof(line), "SP   : %5.1f %%", sp);
    OledDisplay_PrintAt(0, 20, line);

    snprintf(line, sizeof(line), "Band : %.1f..%.1f %%", lower, upper);
    OledDisplay_PrintAt(0, 30, line);

    snprintf(line, sizeof(line), "LED  : %s", led ? "ON " : "OFF");
    OledDisplay_PrintAt(0, 40, line);

    snprintf(line, sizeof(line), "HY   : %5.1f %%", hy);
    OledDisplay_PrintAt(0, 50, line);

    if (fault) OledDisplay_PrintAt(0, 57, "!! SENSOR FAULT !!");
}

static void oledPid(float light, float sp, float outPct,
                    float kp, float ki, float kd, int fault)
{
    char line[32];
    float err = sp - light;

    OledDisplay_PrintAt(16, 0, "  PID LIGHT CTRL");

    snprintf(line, sizeof(line), "Light: %5.1f %%", light);
    OledDisplay_PrintAt(0, 10, line);

    snprintf(line, sizeof(line), "SP   : %5.1f %%", sp);
    OledDisplay_PrintAt(0, 20, line);

    snprintf(line, sizeof(line), "Out  : %5.1f %%", outPct);
    OledDisplay_PrintAt(0, 30, line);

    snprintf(line, sizeof(line), "Err  : %+5.1f %%", err);
    OledDisplay_PrintAt(0, 40, line);

    /* Fit all three gains on one row: "Kp=X.X Ki=X.X Kd=X.X" (21 chars) */
    snprintf(line, sizeof(line), "Kp=%.1f Ki=%.1f Kd=%.1f", kp, ki, kd);
    OledDisplay_PrintAt(0, 50, line);

    if (fault) OledDisplay_PrintAt(0, 57, "!! SENSOR FAULT !!");
}

/* ── Task ────────────────────────────────────────────────────────────────── */

void TaskReporter_Task(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    int cycle = 0;

    for (;;) {
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

        /* ── OLED ─────────────────────────────────────────────────────── */
        OledDisplay_Clear();
        OledDisplay_SetTextSize(1);

        if (mode == CTRL_MODE_PID) {
            oledPid(light, sp, outPct, kp, ki, kd, fault);
        } else {
            oledOnOff(light, sp, hy, led, fault);
        }

        OledDisplay_Update();

        /* ── Serial Plotter ───────────────────────────────────────────── */
        /* Unified 3-channel format works for both modes:
         *   Output = PID duty %  in PID mode
         *   Output = 0 or 100    in ON-OFF mode */
        printf(">Light:%.1f,SP:%.1f,Output:%.1f\n", light, sp, outPct);

        /* ── Full console report ──────────────────────────────────────── */
        if (++cycle >= FULL_REPORT_EVERY_N) {
            cycle = 0;
            printf("--------- REPORT (%s) ----------\n",
                   mode == CTRL_MODE_PID ? "PID" : "ON-OFF");
            printf("  Light  : %.1f %%\n",  light);
            printf("  SP     : %.1f %%\n",  sp);
            printf("  Output : %.1f %%\n",  outPct);
            if (mode == CTRL_MODE_ONOFF) {
                printf("  HY     : %.1f %%  [%.1f .. %.1f]\n",
                       hy, sp - hy*0.5f, sp + hy*0.5f);
                printf("  LED    : %s\n", led ? "ON" : "OFF");
            } else {
                printf("  Kp/Ki/Kd: %.3f / %.3f / %.3f\n", kp, ki, kd);
                printf("  Error  : %.1f %%\n", sp - light);
            }
            if (fault) printf("  *** SENSOR FAULT ***\n");
            printf("---------------------------\n");
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(REPORTER_PERIOD_MS));
    }
}
