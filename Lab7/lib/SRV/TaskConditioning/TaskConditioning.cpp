#include "TaskConditioning.h"
#include "ActuatorData.h"
#include "LedDriver.h"

/* ── Saturation (clamp to valid range) ── */
static float Saturate(float val)
{
    if (val < (float)SPEED_MIN) return (float)SPEED_MIN;
    if (val > (float)SPEED_MAX) return (float)SPEED_MAX;
    return val;
}

/* ── Median filter (removes impulsive command noise) ── */
static float medianBuf[MEDIAN_WINDOW];
static uint8_t medianIdx = 0;
static uint8_t medianFilled = 0;

static void MedianFilter_Push(float val)
{
    medianBuf[medianIdx] = val;
    medianIdx = (medianIdx + 1) % MEDIAN_WINDOW;
    if (medianFilled < MEDIAN_WINDOW) medianFilled++;
}

static float MedianFilter_Get(void)
{
    if (medianFilled == 0) return 0.0f;

    float sorted[MEDIAN_WINDOW];
    uint8_t n = medianFilled;
    for (uint8_t i = 0; i < n; i++) sorted[i] = medianBuf[i];

    /* Insertion sort (n <= 5) */
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

/* ── Exponential Moving Average ── */
static float emaValue = 0.0f;
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
static AlertState_t EvaluateAlert(float conditioned)
{
    if (conditioned >= (float)OVERLOAD_THRESHOLD)
        return ALERT_OVERLOAD;
    if (conditioned <= (float)SPEED_MIN || conditioned >= (float)SPEED_MAX)
        return ALERT_LIMIT_REACHED;
    return ALERT_NONE;
}

/*
 * TaskConditioning - signal-conditioning pipeline for actuator commands:
 *   1. Saturation (clamp to 0-100%)
 *   2. Median filter (window=5, removes impulsive noise)
 *   3. EMA smoothing (alpha=0.3)
 *   4. Ramping (increment currentSpeed toward conditioned by RAMP_STEP)
 *   5. Alert evaluation
 */
void TaskConditioning_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        /* Wait for new command or timeout */
        xSemaphoreTake(xSemNewCommand, pdMS_TO_TICKS(CONDITIONING_PERIOD_MS));

        /* Read target */
        float target = (float)ActuatorData_GetTargetSpeed();

        /* Check emergency stop */
        if (ActuatorData_GetEmergencyStop()) {
            /* Immediate stop: bypass all filtering */
            ActuatorData_SetConditionedSpeed(0.0f);
            ActuatorData_SetCurrentSpeed(0.0f);
            ActuatorData_SetMotorState(MOTOR_IDLE);
            ActuatorData_SetAlert(ALERT_NONE);

            /* Reset filters */
            medianIdx = 0;
            medianFilled = 0;
            emaInitialised = 0;
            emaValue = 0.0f;

            LedDriver_RedOn();
            LedDriver_GreenOff();

            vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CONDITIONING_PERIOD_MS));
            continue;
        }

        /* 1. Saturation */
        float saturated = Saturate(target);

        /* 2. Median filter */
        MedianFilter_Push(saturated);
        float filtered = MedianFilter_Get();

        /* 3. EMA smoothing */
        float conditioned = EMA_Update(filtered);
        ActuatorData_SetConditionedSpeed(conditioned);

        /* 4. Ramping */
        float currentSpeed = ActuatorData_GetCurrentSpeed();
        float diff = conditioned - currentSpeed;

        if (diff > (float)RAMP_STEP) {
            currentSpeed += (float)RAMP_STEP;
            ActuatorData_SetMotorState(MOTOR_RAMP_UP);
        } else if (diff < -(float)RAMP_STEP) {
            currentSpeed -= (float)RAMP_STEP;
            ActuatorData_SetMotorState(MOTOR_RAMP_DOWN);
        } else {
            currentSpeed = conditioned;
            if (currentSpeed < 1.0f) {
                ActuatorData_SetMotorState(MOTOR_IDLE);
            } else {
                ActuatorData_SetMotorState(MOTOR_RUNNING);
            }
        }
        ActuatorData_SetCurrentSpeed(currentSpeed);

        /* 5. Alert evaluation */
        AlertState_t alert = EvaluateAlert(conditioned);
        ActuatorData_SetAlert(alert);

        /* LED feedback */
        if (alert != ALERT_NONE) {
            LedDriver_GreenOff();
            LedDriver_RedOn();
        } else {
            LedDriver_RedOff();
            LedDriver_GreenOn();
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(CONDITIONING_PERIOD_MS));
    }
}
