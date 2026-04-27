#pragma once

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

/* ── Task periods ─────────────────────────────────────────────────────────── */
#define ACQUISITION_PERIOD_MS   100u   /* LDR is fast; 100 ms is safe          */
#define CONTROL_PERIOD_MS       200u   /* Hysteresis evaluation rate            */
#define COMMAND_PERIOD_MS       100u   /* Serial input polling rate             */
#define REPORTER_PERIOD_MS      500u   /* Display/log refresh rate              */

/* ── Default control parameters ───────────────────────────────────────────── */
#define DEFAULT_SETPOINT        50.0f  /* % light level                         */
#define DEFAULT_HYSTERESIS      10.0f  /* % total dead band (±5 % each side)    */
#define SETPOINT_MIN             0.0f
#define SETPOINT_MAX           100.0f
#define HYSTERESIS_MIN           1.0f
#define HYSTERESIS_MAX          50.0f

/* ── Synchronisation primitive ────────────────────────────────────────────── */
extern SemaphoreHandle_t xSystemMutex;

/* ── Lifecycle ────────────────────────────────────────────────────────────── */
void SystemData_Init(void);

/* ── Light level (written by TaskAcquisition) ─────────────────────────────── */
void  SystemData_SetLightLevel(float pct);
float SystemData_GetLightLevel(void);

/* ── Control parameters (written by TaskCommand) ──────────────────────────── */
void  SystemData_SetSetpoint(float sp);
float SystemData_GetSetpoint(void);
void  SystemData_SetHysteresis(float hy);
float SystemData_GetHysteresis(void);

/* ── LED state (written by TaskControl) ───────────────────────────────────── */
void SystemData_SetLedState(int state);  /* 0 = off, 1 = on */
int  SystemData_GetLedState(void);

/* ── Fault flag (written by TaskAcquisition / TaskControl) ───────────────── */
void SystemData_SetFault(int fault);     /* 0 = ok, 1 = fault */
int  SystemData_GetFault(void);
