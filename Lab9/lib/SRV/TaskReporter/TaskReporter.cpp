/*
 * TaskReporter – display and serial logging.
 *
 * Period justification: 500 ms keeps the OLED and serial plotter responsive
 * while leaving CPU headroom for higher-priority tasks.
 *
 * Uses printf() exclusively (no Serial.* calls).
 *
 * Outputs:
 *   1. OLED 128×64 (text size 1 → ~21 chars × 8 rows, 8 px row height)
 *   2. Arduino Serial Plotter line  ">name:value" CSV format
 *   3. Verbose console summary every FULL_REPORT_EVERY_N cycles (~5 s)
 */
#include "TaskReporter.h"
#include "SystemData.h"
#include "OledDisplay.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <stdio.h>

#define FULL_REPORT_EVERY_N  10  /* every 5 s at 500 ms period */

void TaskReporter_Task(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    int  cycle = 0;
    char line[32];

    for (;;) {
        float light = SystemData_GetLightLevel();
        float sp    = SystemData_GetSetpoint();
        float hy    = SystemData_GetHysteresis();
        int   led   = SystemData_GetLedState();
        int   fault = SystemData_GetFault();
        float upper = sp + hy * 0.5f;
        float lower = sp - hy * 0.5f;

        /* ── OLED (128×64, text size 1 = 6×8 px/char) ───────────────────── */
        OledDisplay_Clear();
        OledDisplay_SetTextSize(1);

        OledDisplay_PrintAt(10, 0,  "ON-OFF LIGHT CTRL");   /* y= 0  title      */

        snprintf(line, sizeof(line), "Light: %5.1f %%", light);
        OledDisplay_PrintAt(0, 10, line);                    /* y=10 light level */

        snprintf(line, sizeof(line), "SP   : %5.1f %%", sp);
        OledDisplay_PrintAt(0, 20, line);                    /* y=20 setpoint    */

        snprintf(line, sizeof(line), "Band : %.1f..%.1f %%", lower, upper);
        OledDisplay_PrintAt(0, 30, line);                    /* y=30 dead band   */

        snprintf(line, sizeof(line), "LED  : %s", led ? "ON " : "OFF");
        OledDisplay_PrintAt(0, 40, line);                    /* y=40 LED state   */

        snprintf(line, sizeof(line), "HY   : %5.1f %%", hy);
        OledDisplay_PrintAt(0, 50, line);                    /* y=50 hysteresis  */

        if (fault) {
            OledDisplay_PrintAt(0, 57, "!! SENSOR FAULT !!");
        }

        OledDisplay_Update();

        /* ── Serial Plotter (Arduino IDE compatible) ─────────────────────── */
        printf(">Light:%.1f,SP:%.1f,Upper:%.1f,Lower:%.1f,LED:%d\n",
               light, sp, upper, lower, led);

        /* ── Full console report ─────────────────────────────────────────── */
        if (++cycle >= FULL_REPORT_EVERY_N) {
            cycle = 0;
            printf("--------- REPORT ----------\n");
            printf("  Light : %.1f %%\n",  light);
            printf("  SP    : %.1f %%\n",  sp);
            printf("  Band  : [%.1f .. %.1f] %%  HY=%.1f\n", lower, upper, hy);
            printf("  LED   : %s\n",  led   ? "ON"  : "OFF");
            if (fault) printf("  *** SENSOR FAULT ***\n");
            printf("---------------------------\n");
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(REPORTER_PERIOD_MS));
    }
}
