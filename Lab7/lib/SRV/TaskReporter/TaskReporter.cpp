#include "TaskReporter.h"
#include "OledDisplay.h"
#include "ActuatorData.h"
#include <Arduino.h>
#include <stdio.h>

static const char *MotorStateName(MotorState_t st)
{
    switch (st) {
    case MOTOR_IDLE:      return "IDLE";
    case MOTOR_RAMP_UP:   return "RAMP_UP";
    case MOTOR_RUNNING:   return "RUNNING";
    case MOTOR_RAMP_DOWN: return "RAMP_DN";
    default:              return "???";
    }
}

static const char *AlertName(AlertState_t a)
{
    switch (a) {
    case ALERT_NONE:           return "NONE";
    case ALERT_LIMIT_REACHED:  return "LIMIT";
    case ALERT_OVERLOAD:       return "OVERLD";
    default:                   return "???";
    }
}

void TaskReporter_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();
    uint32_t reportCounter = 0;

    for (;;) {
        int16_t      target = ActuatorData_GetTargetSpeed();
        float        cond   = ActuatorData_GetConditionedSpeed();
        float        curr   = ActuatorData_GetCurrentSpeed();
        Direction_t  dir    = ActuatorData_GetDirection();
        MotorState_t mst    = ActuatorData_GetMotorState();
        RelayState_t relay  = ActuatorData_GetRelayState();
        AlertState_t alert  = ActuatorData_GetAlert();

        /* ===== OLED 128x64 (8 lines x 21 chars at size 1) ===== */
        char buf[22];
        OledDisplay_Clear();
        OledDisplay_SetTextSize(1);

        OledDisplay_PrintAt(4, 0, "-- Actuator Ctrl --");

        snprintf(buf, sizeof(buf), "Motor: %-7s %s",
                 MotorStateName(mst),
                 dir == DIR_FORWARD ? "FWD" : "REV");
        OledDisplay_PrintAt(0, 10, buf);

        snprintf(buf, sizeof(buf), "Target:  %d%%", target);
        OledDisplay_PrintAt(0, 20, buf);

        snprintf(buf, sizeof(buf), "Cond:    %.0f%%", cond);
        OledDisplay_PrintAt(0, 30, buf);

        snprintf(buf, sizeof(buf), "Current: %.0f%%", curr);
        OledDisplay_PrintAt(0, 40, buf);

        snprintf(buf, sizeof(buf), "Relay: %s  Alrt:%s",
                 relay == RELAY_ON ? "ON " : "OFF",
                 AlertName(alert));
        OledDisplay_PrintAt(0, 50, buf);

        snprintf(buf, sizeof(buf), "OVL:%d Ramp:%d/cyc",
                 OVERLOAD_THRESHOLD, RAMP_STEP);
        OledDisplay_PrintAt(0, 58, buf);

        OledDisplay_Update();

        /* ===== Serial Plotter (>name:value format) ===== */
        Serial.print(">");
        Serial.print("target:");
        Serial.print(target);
        Serial.print(",cond:");
        Serial.print(cond, 1);
        Serial.print(",current:");
        Serial.print(curr, 1);
        Serial.print(",relay:");
        Serial.print(relay == RELAY_ON ? 1 : 0);
        Serial.println();

        /* ===== Structured report (every FULL_REPORT_EVERY cycles = 5s) ===== */
        if ((reportCounter++ % FULL_REPORT_EVERY) == 0) {
            printf("\n===== ACTUATOR REPORT (t=%lu ms) =====\n",
                   (unsigned long)(xTaskGetTickCount() * portTICK_PERIOD_MS));
            printf("Target speed  : %d%%\n", target);
            printf("Conditioned   : %.1f%%  (median=%d, EMA alpha=%.2f)\n",
                   cond, MEDIAN_WINDOW, EMA_ALPHA);
            printf("Current speed : %.1f%%  (ramp step=%d/cyc)\n",
                   curr, RAMP_STEP);
            printf("Direction     : %s\n",
                   dir == DIR_FORWARD ? "FORWARD" : "REVERSE");
            printf("Motor state   : %s\n", MotorStateName(mst));
            printf("Relay         : %s\n",
                   relay == RELAY_ON ? "ON" : "OFF");
            printf("Alert         : %s  (overload >= %d%%)\n",
                   AlertName(alert), OVERLOAD_THRESHOLD);
            printf("PWM duty      : %d/255\n",
                   (int)((curr * 255.0f) / 100.0f));
            printf("======================================\n\n");
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(REPORTER_PERIOD_MS));
    }
}
