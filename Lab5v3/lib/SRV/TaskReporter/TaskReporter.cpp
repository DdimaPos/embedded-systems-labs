#include "TaskReporter.h"
#include "SensorData.h"
#include "LcdDisplay.h"
#include <stdio.h>

#define REPORTER_PERIOD_MS  500
#define FULL_REPORT_EVERY   10  // 10 x 500ms = 5 sec

void TaskReporter_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint32_t reportCounter = 0;

    for (;;) {
        float        temp   = SensorData_GetTemperature();
        int          tempOk = SensorData_IsTempValid();
        AlertState_t tAlert = SensorData_GetTempAlert();
        uint8_t      tDbg   = SensorData_GetTempDebounce();

        // ===== LCD =====
        char line1[17], line2[17];
        if (tempOk) {
            snprintf(line1, sizeof(line1), "Temp: %.1f C    ", temp);
        } else {
            snprintf(line1, sizeof(line1), "Temp: ERROR     ");
        }
        snprintf(line2, sizeof(line2), "Alert: %-8s",
                 (tAlert == ALERT_ON) ? "ON" : "OFF");

        LcdDisplay_PrintAt(0, 0, line1);
        LcdDisplay_PrintAt(0, 1, line2);

        // ===== Serial plotter (CSV) =====
        // Format: temp,threshHigh,threshLow
        printf("%.1f,%.1f,%.1f\n", temp, TEMP_THRESH_HIGH, TEMP_THRESH_LOW);

        // ===== Structured report every 5 seconds =====
        if ((reportCounter++ % FULL_REPORT_EVERY) == 0) {
            printf("\n--- SENSOR REPORT (t=%lu ms) ---\n",
                   xTaskGetTickCount() * portTICK_PERIOD_MS);
            printf("Temp  : %.1f C (valid=%d, alert=%s, dbg=%d)\n",
                   temp, tempOk, (tAlert ? "ON" : "OFF"), tDbg);
            printf("Thresh: ON >= %.1f C, OFF <= %.1f C\n",
                   TEMP_THRESH_HIGH, TEMP_THRESH_LOW);
            printf("--------------------------------\n\n");
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(REPORTER_PERIOD_MS));
    }
}
