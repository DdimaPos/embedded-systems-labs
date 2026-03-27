#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <stdint.h>

/* ── Task periods (ms) ── */
#define COMMAND_PERIOD_MS       50
#define CONDITIONING_PERIOD_MS  50
#define ACTUATOR_PERIOD_MS      50
#define REPORTER_PERIOD_MS      500
#define FULL_REPORT_EVERY       10   /* 10 x 500ms = 5s */

/* ── Speed limits (%) ── */
#define SPEED_MIN   0
#define SPEED_MAX   100

/* ── Signal conditioning ── */
#define MEDIAN_WINDOW       5
#define EMA_ALPHA           0.3f
#define RAMP_STEP           2    /* % per 50ms cycle -> ~2.5s full ramp */
#define OVERLOAD_THRESHOLD  95   /* % */

/* ── Direction ── */
typedef enum { DIR_FORWARD = 0, DIR_REVERSE = 1 } Direction_t;

/* ── Motor state machine ── */
typedef enum {
    MOTOR_IDLE = 0,
    MOTOR_RAMP_UP,
    MOTOR_RUNNING,
    MOTOR_RAMP_DOWN
} MotorState_t;

/* ── Relay state ── */
typedef enum { RELAY_OFF = 0, RELAY_ON = 1 } RelayState_t;

/* ── Alert state ── */
typedef enum {
    ALERT_NONE = 0,
    ALERT_LIMIT_REACHED,
    ALERT_OVERLOAD
} AlertState_t;

/* ── Synchronisation ── */
extern SemaphoreHandle_t xSemNewCommand;
extern SemaphoreHandle_t xActuatorMutex;

/* ── Init ── */
void ActuatorData_Init(void);

/* ── Target speed (0-100%, raw from command) ── */
void    ActuatorData_SetTargetSpeed(int16_t val);
int16_t ActuatorData_GetTargetSpeed(void);

/* ── Conditioned speed (after median + EMA) ── */
void    ActuatorData_SetConditionedSpeed(float val);
float   ActuatorData_GetConditionedSpeed(void);

/* ── Current speed (after ramping, actually applied) ── */
void    ActuatorData_SetCurrentSpeed(float val);
float   ActuatorData_GetCurrentSpeed(void);

/* ── Direction ── */
void        ActuatorData_SetDirection(Direction_t dir);
Direction_t ActuatorData_GetDirection(void);

/* ── Motor state ── */
void         ActuatorData_SetMotorState(MotorState_t st);
MotorState_t ActuatorData_GetMotorState(void);

/* ── Relay ── */
void        ActuatorData_SetRelayTarget(RelayState_t st);
RelayState_t ActuatorData_GetRelayTarget(void);
void        ActuatorData_SetRelayState(RelayState_t st);
RelayState_t ActuatorData_GetRelayState(void);

/* ── Alert ── */
void        ActuatorData_SetAlert(AlertState_t a);
AlertState_t ActuatorData_GetAlert(void);

/* ── Emergency stop flag ── */
void ActuatorData_SetEmergencyStop(uint8_t flag);
uint8_t ActuatorData_GetEmergencyStop(void);
