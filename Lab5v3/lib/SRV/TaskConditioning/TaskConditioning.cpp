#include "TaskConditioning.h"
#include "SensorData.h"
#include "LedDriver.h"

// Apply hysteresis + debounce for the temperature channel.
static AlertState_t ProcessChannel(
    float value, float threshHigh, float threshLow,
    AlertState_t currentAlert, uint8_t *debounce)
{
    // Hysteresis
    AlertState_t desired;
    if      (value >= threshHigh) desired = ALERT_ON;
    else if (value <= threshLow)  desired = ALERT_OFF;
    else                          desired = currentAlert;

    // Debounce
    if (desired != currentAlert) {
        (*debounce)++;
        if (*debounce >= DEBOUNCE_COUNT) {
            *debounce = 0;
            return desired;  // transition confirmed
        }
    } else {
        *debounce = 0;
    }
    return currentAlert;  // no change yet
}

void TaskConditioning_Task(void *pvParameters)
{
    for (;;) {
        // Block until Acquisition signals new data (200ms watchdog timeout)
        if (xSemaphoreTake(xSemNewData, pdMS_TO_TICKS(200)) != pdTRUE) {
            continue;
        }

        // Process temperature channel
        if (SensorData_IsTempValid()) {
            float        temp   = SensorData_GetTemperature();
            AlertState_t tAlert = SensorData_GetTempAlert();
            uint8_t      tDbg   = SensorData_GetTempDebounce();

            AlertState_t newAlert = ProcessChannel(
                temp, TEMP_THRESH_HIGH, TEMP_THRESH_LOW, tAlert, &tDbg);

            SensorData_SetTempDebounce(tDbg);
            if (newAlert != tAlert) {
                SensorData_SetTempAlert(newAlert);
            }
        }

        // Apply LEDs: Green = OK, Red = temperature alert
        AlertState_t ta = SensorData_GetTempAlert();
        if (ta == ALERT_ON) {
            LedDriver_GreenOff();
            LedDriver_RedOn();
        } else {
            LedDriver_RedOff();
            LedDriver_GreenOn();
        }
    }
}
