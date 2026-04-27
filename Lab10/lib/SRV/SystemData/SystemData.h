#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

/* ── Task periods ─────────────────────────────────────────────────────────── */
#define ACQUISITION_PERIOD_MS   100u   /* LDR is fast; 100 ms is safe          */
#define CONTROL_PERIOD_MS       200u   /* Control evaluation rate               */
#define COMMAND_PERIOD_MS       100u   /* Serial input polling rate             */
#define REPORTER_PERIOD_MS      500u   /* Display/log refresh rate              */

/* ── Default control parameters ───────────────────────────────────────────── */
#define DEFAULT_SETPOINT        50.0f  /* % light level                         */
#define DEFAULT_HYSTERESIS      10.0f  /* % total dead band (±5 % each side)    */
#define SETPOINT_MIN             0.0f
#define SETPOINT_MAX           100.0f
#define HYSTERESIS_MIN           1.0f
#define HYSTERESIS_MAX          50.0f

/* ── Default PID tunings ───────────────────────────────────────────────────── */
#define DEFAULT_KP               3.0f  /* Proportional gain                     */
#define DEFAULT_KI               0.5f  /* Integral gain                         */
#define DEFAULT_KD               0.2f  /* Derivative gain                       */
#define KP_MIN                   0.0f
#define KP_MAX                 100.0f
#define KI_MIN                   0.0f
#define KI_MAX                  50.0f
#define KD_MIN                   0.0f
#define KD_MAX                  50.0f

/* ── Control mode ─────────────────────────────────────────────────────────── */
typedef enum { CTRL_MODE_ONOFF = 0, CTRL_MODE_PID = 1 } ControlMode_t;

/* ── Synchronisation primitive ────────────────────────────────────────────── */
extern SemaphoreHandle_t xSystemMutex;

/* ── Lifecycle ────────────────────────────────────────────────────────────── */
void SystemData_Init(void);

/* ── Light level (written by TaskAcquisition) ─────────────────────────────── */
void  SystemData_SetLightLevel(float pct);
float SystemData_GetLightLevel(void);

/* ── Control parameters (written by TaskCommand) ──────────────────────────── */
void         SystemData_SetSetpoint(float sp);
float        SystemData_GetSetpoint(void);
void         SystemData_SetHysteresis(float hy);
float        SystemData_GetHysteresis(void);
void         SystemData_SetControlMode(ControlMode_t mode);
ControlMode_t SystemData_GetControlMode(void);
void         SystemData_SetKp(float kp);
float        SystemData_GetKp(void);
void         SystemData_SetKi(float ki);
float        SystemData_GetKi(void);
void         SystemData_SetKd(float kd);
float        SystemData_GetKd(void);

/* ── Actuator state (written by TaskControl) ──────────────────────────────── */
void  SystemData_SetLedState(int state);  /* 0 = off, 1 = on  (ON-OFF mode)   */
int   SystemData_GetLedState(void);
void  SystemData_SetPidOutput(float pct); /* 0-100 % actuator output          */
float SystemData_GetPidOutput(void);

/* ── Fault flag (written by TaskAcquisition / TaskControl) ───────────────── */
void SystemData_SetFault(int fault);      /* 0 = ok, 1 = fault                */
int  SystemData_GetFault(void);
