#include "TaskConditioning.h"
#include "SensorData.h"
#include "LedDriver.h"

/* ── Median filter (salt-and-pepper removal) ── */
static float medianBuf[MEDIAN_WINDOW];
static uint8_t medianIdx = 0;
static uint8_t medianFilled = 0;

static void MedianFilter_Push(float val)
{
    medianBuf[medianIdx] = val;
    medianIdx = (medianIdx + 1) % MEDIAN_WINDOW;
    if (medianFilled < MEDIAN_WINDOW) medianFilled++;
}

/* Simple insertion-sort on a small copy → return median */
static float MedianFilter_Get(void)
{
    if (medianFilled == 0) return 0.0f;

    float sorted[MEDIAN_WINDOW];
    uint8_t n = medianFilled;
    for (uint8_t i = 0; i < n; i++) sorted[i] = medianBuf[i];

    /* Insertion sort (n ≤ 5) */
    for (uint8_t i = 1; i < n; i++) {
        float key = sorted[i];
        int j = i - 1;
        while (j >= 0 && sorted[j] > key) {
            sorted[j + 1] = sorted[j];
            j--;
        }
        sorted[j + 1] = key;
    }
    return sorted[n / 2];
}

/* ── Saturation (clamp) ── */
static float Saturate(float val)
{
    if (val < SATURATION_LOW)  return SATURATION_LOW;
    if (val > SATURATION_HIGH) return SATURATION_HIGH;
    return val;
}

/* ── Exponential Moving Average ── */
static float emaValue = 20.0f;
static int   emaInitialised = 0;

static float EMA_Update(float newVal)
{
    if (!emaInitialised) {
        emaValue = newVal;
        emaInitialised = 1;
    } else {
        emaValue = EMA_ALPHA * newVal + (1.0f - EMA_ALPHA) * emaValue;
    }
    return emaValue;
}

/* ── Alert evaluation ── */
static AlertState_t EvaluateAlert(float processed, AlertState_t current)
{
    if      (processed >= TEMP_THRESH_HIGH) return ALERT_ON;
    else if (processed <= TEMP_THRESH_LOW)  return ALERT_OFF;
    return current;  /* hysteresis band → keep previous state */
}

/*
 * TaskConditioning – signal-conditioning pipeline:
 *   1. Saturation (clamp to physical range)
 *   2. Median filter (salt-and-pepper / outlier removal)
 *   3. Weighted average (EMA smoothing)
 *   4. Threshold alert evaluation
 */
void TaskConditioning_Task(void *pvParameters)
{
    for (;;) {
        /* Block until Acquisition signals new data */
        if (xSemaphoreTake(xSemNewData, pdMS_TO_TICKS(200)) != pdTRUE)
            continue;

        if (!SensorData_IsTempValid())
            continue;

        float raw = SensorData_GetRawTemp();

        /* 1. Saturation */
        float saturated = Saturate(raw);

        /* 2. Median filter (removes salt-and-pepper noise) */
        MedianFilter_Push(saturated);
        float filtered = MedianFilter_Get();
        SensorData_SetFilteredTemp(filtered);

        /* 3. Weighted average (EMA) */
        float averaged = EMA_Update(filtered);
        SensorData_SetAveragedTemp(averaged);

        /* 4. Alert evaluation on the median-filtered value (not EMA,
         *    which lags too much and can keep the alert stuck in the
         *    hysteresis band) */
        AlertState_t currentAlert = SensorData_GetTempAlert();
        AlertState_t newAlert = EvaluateAlert(filtered, currentAlert);
        if (newAlert != currentAlert) {
            SensorData_SetTempAlert(newAlert);
        }

        /* LED feedback: Green = OK, Red = alert */
        if (newAlert == ALERT_ON) {
            LedDriver_GreenOff();
            LedDriver_RedOn();
        } else {
            LedDriver_RedOff();
            LedDriver_GreenOn();
        }
    }
}
