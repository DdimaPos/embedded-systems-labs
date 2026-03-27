#include "ActuatorData.h"

SemaphoreHandle_t xSemNewCommand  = NULL;
SemaphoreHandle_t xActuatorMutex  = NULL;

static int16_t      gTargetSpeed      = 0;
static float        gConditionedSpeed = 0.0f;
static float        gCurrentSpeed     = 0.0f;
static Direction_t  gDirection        = DIR_FORWARD;
static MotorState_t gMotorState       = MOTOR_IDLE;
static RelayState_t gRelayTarget      = RELAY_OFF;
static RelayState_t gRelayState       = RELAY_OFF;
static AlertState_t gAlert            = ALERT_NONE;
static uint8_t      gEmergencyStop    = 0;

void ActuatorData_Init(void)
{
    xSemNewCommand  = xSemaphoreCreateBinary();
    xActuatorMutex  = xSemaphoreCreateMutex();
}

/* ── Target speed ── */
void ActuatorData_SetTargetSpeed(int16_t val)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    gTargetSpeed = val;
    xSemaphoreGive(xActuatorMutex);
}

int16_t ActuatorData_GetTargetSpeed(void)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    int16_t val = gTargetSpeed;
    xSemaphoreGive(xActuatorMutex);
    return val;
}

/* ── Conditioned speed ── */
void ActuatorData_SetConditionedSpeed(float val)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    gConditionedSpeed = val;
    xSemaphoreGive(xActuatorMutex);
}

float ActuatorData_GetConditionedSpeed(void)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    float val = gConditionedSpeed;
    xSemaphoreGive(xActuatorMutex);
    return val;
}

/* ── Current speed ── */
void ActuatorData_SetCurrentSpeed(float val)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    gCurrentSpeed = val;
    xSemaphoreGive(xActuatorMutex);
}

float ActuatorData_GetCurrentSpeed(void)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    float val = gCurrentSpeed;
    xSemaphoreGive(xActuatorMutex);
    return val;
}

/* ── Direction ── */
void ActuatorData_SetDirection(Direction_t dir)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    gDirection = dir;
    xSemaphoreGive(xActuatorMutex);
}

Direction_t ActuatorData_GetDirection(void)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    Direction_t val = gDirection;
    xSemaphoreGive(xActuatorMutex);
    return val;
}

/* ── Motor state ── */
void ActuatorData_SetMotorState(MotorState_t st)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    gMotorState = st;
    xSemaphoreGive(xActuatorMutex);
}

MotorState_t ActuatorData_GetMotorState(void)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    MotorState_t val = gMotorState;
    xSemaphoreGive(xActuatorMutex);
    return val;
}

/* ── Relay target ── */
void ActuatorData_SetRelayTarget(RelayState_t st)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    gRelayTarget = st;
    xSemaphoreGive(xActuatorMutex);
}

RelayState_t ActuatorData_GetRelayTarget(void)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    RelayState_t val = gRelayTarget;
    xSemaphoreGive(xActuatorMutex);
    return val;
}

/* ── Relay state ── */
void ActuatorData_SetRelayState(RelayState_t st)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    gRelayState = st;
    xSemaphoreGive(xActuatorMutex);
}

RelayState_t ActuatorData_GetRelayState(void)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    RelayState_t val = gRelayState;
    xSemaphoreGive(xActuatorMutex);
    return val;
}

/* ── Alert ── */
void ActuatorData_SetAlert(AlertState_t a)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    gAlert = a;
    xSemaphoreGive(xActuatorMutex);
}

AlertState_t ActuatorData_GetAlert(void)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    AlertState_t val = gAlert;
    xSemaphoreGive(xActuatorMutex);
    return val;
}

/* ── Emergency stop ── */
void ActuatorData_SetEmergencyStop(uint8_t flag)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    gEmergencyStop = flag;
    xSemaphoreGive(xActuatorMutex);
}

uint8_t ActuatorData_GetEmergencyStop(void)
{
    xSemaphoreTake(xActuatorMutex, portMAX_DELAY);
    uint8_t val = gEmergencyStop;
    xSemaphoreGive(xActuatorMutex);
    return val;
}
