#include "SystemData.h"

SemaphoreHandle_t xSystemMutex = NULL;

static float s_lightLevel = 0.0f;
static float s_setpoint   = DEFAULT_SETPOINT;
static float s_hysteresis = DEFAULT_HYSTERESIS;
static int   s_ledState   = 0;
static int   s_fault      = 0;

/* Helper – take mutex with short timeout, returns pdTRUE on success */
static inline BaseType_t take(void)
{
    return xSemaphoreTake(xSystemMutex, pdMS_TO_TICKS(10));
}

void SystemData_Init(void)
{
    xSystemMutex = xSemaphoreCreateMutex();
    configASSERT(xSystemMutex != NULL);
}

/* ── Light level ─────────────────────────────────────────────────────────── */

void SystemData_SetLightLevel(float pct)
{
    if (take() == pdTRUE) { s_lightLevel = pct; xSemaphoreGive(xSystemMutex); }
}

float SystemData_GetLightLevel(void)
{
    float v = 0.0f;
    if (take() == pdTRUE) { v = s_lightLevel; xSemaphoreGive(xSystemMutex); }
    return v;
}

/* ── Control parameters ──────────────────────────────────────────────────── */

void SystemData_SetSetpoint(float sp)
{
    if (sp < SETPOINT_MIN) sp = SETPOINT_MIN;
    if (sp > SETPOINT_MAX) sp = SETPOINT_MAX;
    if (take() == pdTRUE) { s_setpoint = sp; xSemaphoreGive(xSystemMutex); }
}

float SystemData_GetSetpoint(void)
{
    float v = DEFAULT_SETPOINT;
    if (take() == pdTRUE) { v = s_setpoint; xSemaphoreGive(xSystemMutex); }
    return v;
}

void SystemData_SetHysteresis(float hy)
{
    if (hy < HYSTERESIS_MIN) hy = HYSTERESIS_MIN;
    if (hy > HYSTERESIS_MAX) hy = HYSTERESIS_MAX;
    if (take() == pdTRUE) { s_hysteresis = hy; xSemaphoreGive(xSystemMutex); }
}

float SystemData_GetHysteresis(void)
{
    float v = DEFAULT_HYSTERESIS;
    if (take() == pdTRUE) { v = s_hysteresis; xSemaphoreGive(xSystemMutex); }
    return v;
}

/* ── LED state ───────────────────────────────────────────────────────────── */

void SystemData_SetLedState(int state)
{
    if (take() == pdTRUE) { s_ledState = state; xSemaphoreGive(xSystemMutex); }
}

int SystemData_GetLedState(void)
{
    int v = 0;
    if (take() == pdTRUE) { v = s_ledState; xSemaphoreGive(xSystemMutex); }
    return v;
}

/* ── Fault flag ──────────────────────────────────────────────────────────── */

void SystemData_SetFault(int fault)
{
    if (take() == pdTRUE) { s_fault = fault; xSemaphoreGive(xSystemMutex); }
}

int SystemData_GetFault(void)
{
    int v = 0;
    if (take() == pdTRUE) { v = s_fault; xSemaphoreGive(xSystemMutex); }
    return v;
}
