#include "TaskReporter.h"
#include "LcdDisplay.h"
#include "OledDisplay.h"
#include "SensorData.h"
#include <Arduino.h>
#include <stdio.h>

static const char *SensorName(SensorId_t id) {
  switch (id) {
  case SENSOR_DHT11:
    return "DHT11";
  default:
    return "UNKNOWN";
  }
}

/*
 * TaskReporter – displays processed values, intermediate stages and
 * alerts on the OLED, LCD and via Serial (printf) at REPORTER_PERIOD_MS.
 */
void TaskReporter_Task(void *pvParameters) {
  TickType_t xLastWakeTime = xTaskGetTickCount();
  uint32_t reportCounter = 0;

  for (;;) {
    float raw = SensorData_GetRawTemp();
    float filtered = SensorData_GetFilteredTemp();
    float averaged = SensorData_GetAveragedTemp();
    int tempOk = SensorData_IsTempValid();
    AlertState_t tAlert = SensorData_GetTempAlert();
    SensorId_t sensor = SensorData_GetActiveSensor();

    /* ===== OLED 128x64 (8 lines x 21 chars at size 1) ===== */
    char buf[22];
    OledDisplay_Clear();
    OledDisplay_SetTextSize(1);

    OledDisplay_PrintAt(10, 0, "-- Temp Monitor --");

    snprintf(buf, sizeof(buf), "Sensor: %s", SensorName(sensor));
    OledDisplay_PrintAt(0, 10, buf);

    if (tempOk) {
      snprintf(buf, sizeof(buf), "Raw:    %.2f C", raw);
      OledDisplay_PrintAt(0, 20, buf);
      snprintf(buf, sizeof(buf), "Median: %.2f C", filtered);
      OledDisplay_PrintAt(0, 30, buf);
      snprintf(buf, sizeof(buf), "EMA:    %.2f C", averaged);
      OledDisplay_PrintAt(0, 40, buf);
    } else {
      OledDisplay_PrintAt(0, 20, "Sensor ERROR");
    }

    snprintf(buf, sizeof(buf), "Alert:%s",
             (tAlert == ALERT_ON) ? "ON " : "OFF");
    OledDisplay_PrintAt(0, 52, buf);

    snprintf(buf, sizeof(buf), "Hi:%.0f Lo:%.0f", TEMP_THRESH_HIGH,
             TEMP_THRESH_LOW);
    OledDisplay_PrintAt(62, 52, buf);

    OledDisplay_Update();

    /* ===== LCD 16x2 (backup) ===== */
    char line1[17], line2[17];
    if (tempOk) {
      snprintf(line1, sizeof(line1), "T:%.1f F:%.1f   ", averaged, filtered);
    } else {
      snprintf(line1, sizeof(line1), "Sensor ERROR    ");
    }
    snprintf(line2, sizeof(line2), "%-7s A:%-3s   ", SensorName(sensor),
             (tAlert == ALERT_ON) ? "ON" : "OFF");

    LcdDisplay_PrintAt(0, 0, line1);
    LcdDisplay_PrintAt(0, 1, line2);

    /* ===== Serial Plotter (>name:value format) ===== */
    Serial.print(">");
    Serial.print("raw:");
    Serial.print(raw, 2);
    Serial.print(",median:");
    Serial.print(filtered, 2);
    Serial.print(",ema:");
    Serial.print(averaged, 2);
    Serial.print(",thresh_high:");
    Serial.print(TEMP_THRESH_HIGH, 1);
    Serial.print(",thresh_low:");
    Serial.print(TEMP_THRESH_LOW, 1);
    Serial.println(); /* \r\n */

    /* ===== Structured report (every FULL_REPORT_EVERY cycles) ===== */
    if ((reportCounter++ % FULL_REPORT_EVERY) == 0) {
      printf("\n===== SENSOR REPORT (t=%lu ms) =====\n",
             (unsigned long)(xTaskGetTickCount() * portTICK_PERIOD_MS));
      printf("Active sensor : %s\n", SensorName(sensor));
      printf("Validity      : %s\n", tempOk ? "OK" : "ERROR");
      printf("Raw           : %.2f C\n", raw);
      printf("Saturated+Med : %.2f C  (median window=%d)\n", filtered,
             MEDIAN_WINDOW);
      printf("EMA averaged  : %.2f C  (alpha=%.2f)\n", averaged, EMA_ALPHA);
      printf("Alert         : %s  (thresholds: ON>=%.1f OFF<=%.1f)\n",
             (tAlert == ALERT_ON) ? "ON" : "OFF", TEMP_THRESH_HIGH,
             TEMP_THRESH_LOW);
      printf("====================================\n\n");
    }

    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(REPORTER_PERIOD_MS));
  }
}
