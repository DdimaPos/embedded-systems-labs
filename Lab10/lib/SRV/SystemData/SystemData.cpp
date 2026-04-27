#include "SystemData.h"

SemaphoreHandle_t xSystemMutex = NULL;

static float        s_lightLevel   = 0.0f;
static float        s_setpoint     = DEFAULT_SETPOINT;
static float        s_hysteresis   = DEFAULT_HYSTERESIS;
static ControlMode_t s_controlMode = CTRL_MODE_ONOFF;
static float        s_kp           = DEFAULT_KP;
static float        s_ki           = DEFAULT_KI;
static float        s_kd           = DEFAULT_KD;
static int          s_ledState     = 0;
static float        s_pidOutput    = 0.0f;
static int          s_fault        = 0;

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

void SystemData_SetControlMode(ControlMode_t mode)
{
    if (take() == pdTRUE) { s_controlMode = mode; xSemaphoreGive(xSystemMutex); }
}
ControlMode_t SystemData_GetControlMode(void)
{
    ControlMode_t v = CTRL_MODE_ONOFF;
    if (take() == pdTRUE) { v = s_controlMode; xSemaphoreGive(xSystemMutex); }
    return v;
}

void SystemData_SetKp(float kp)
{
    if (kp < KP_MIN) kp = KP_MIN;
    if (kp > KP_MAX) kp = KP_MAX;
    if (take() == pdTRUE) { s_kp = kp; xSemaphoreGive(xSystemMutex); }
}
float SystemData_GetKp(void)
{
    float v = DEFAULT_KP;
    if (take() == pdTRUE) { v = s_kp; xSemaphoreGive(xSystemMutex); }
    return v;
}

void SystemData_SetKi(float ki)
{
    if (ki < KI_MIN) ki = KI_MIN;
    if (ki > KI_MAX) ki = KI_MAX;
    if (take() == pdTRUE) { s_ki = ki; xSemaphoreGive(xSystemMutex); }
}
float SystemData_GetKi(void)
{
    float v = DEFAULT_KI;
    if (take() == pdTRUE) { v = s_ki; xSemaphoreGive(xSystemMutex); }
    return v;
}

void SystemData_SetKd(float kd)
{
    if (kd < KD_MIN) kd = KD_MIN;
    if (kd > KD_MAX) kd = KD_MAX;
    if (take() == pdTRUE) { s_kd = kd; xSemaphoreGive(xSystemMutex); }
}
float SystemData_GetKd(void)
{
    float v = DEFAULT_KD;
    if (take() == pdTRUE) { v = s_kd; xSemaphoreGive(xSystemMutex); }
    return v;
}

/* ── Actuator state ──────────────────────────────────────────────────────── */

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

void SystemData_SetPidOutput(float pct)
{
    if (take() == pdTRUE) { s_pidOutput = pct; xSemaphoreGive(xSystemMutex); }
}
float SystemData_GetPidOutput(void)
{
    float v = 0.0f;
    if (take() == pdTRUE) { v = s_pidOutput; xSemaphoreGive(xSystemMutex); }
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
