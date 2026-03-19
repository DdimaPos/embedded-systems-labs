#pragma once
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <stdint.h>

/* ── Configurable parameters ── */
#define ACQUISITION_PERIOD_MS 100  /* DHT11 requires >= 2s between reads   */
#define CONDITIONING_PERIOD_MS 100 /* can equal or be a multiple of above  */
#define REPORTER_PERIOD_MS 500     /* display / report cadence             */
#define FULL_REPORT_EVERY 10       /* 10 x 500ms = 5 s full report         */

/* Saturation limits (°C) – DHT11 range: 0..50 °C */
#define SATURATION_LOW 0.0f
#define SATURATION_HIGH 50.0f

/* Alert thresholds (°C) */
#define TEMP_THRESH_HIGH 28.0f
#define TEMP_THRESH_LOW 25.0f

/* Median-filter window (must be odd) */
#define MEDIAN_WINDOW 5

/* Weighted (exponential) moving average coefficient  (0..1) */
#define EMA_ALPHA 0.2f

/* ── Sensor catalogue ── */
typedef enum {
  SENSOR_DHT11 = 0, /* digital, single-wire            */
  /* add future sensors here, e.g. SENSOR_LM35, SENSOR_NTC */
  SENSOR_COUNT
} SensorId_t;

/* Alert state */
typedef enum { ALERT_OFF = 0, ALERT_ON = 1 } AlertState_t;

/* ── Synchronisation ── */
extern SemaphoreHandle_t xSemNewData;
extern SemaphoreHandle_t xSensorMutex;

void SensorData_Init(void);

/* Sensor selection */
void SensorData_SetActiveSensor(SensorId_t id);
SensorId_t SensorData_GetActiveSensor(void);

/* Raw value (straight from sensor_read) */
void SensorData_SetRawTemp(float val);
float SensorData_GetRawTemp(void);

/* After median filter */
void SensorData_SetFilteredTemp(float val);
float SensorData_GetFilteredTemp(void);

/* After weighted average (EMA) */
void SensorData_SetAveragedTemp(float val);
float SensorData_GetAveragedTemp(void);

/* Validity flag */
void SensorData_SetTempValid(int valid);
int SensorData_IsTempValid(void);

/* Alert */
void SensorData_SetTempAlert(AlertState_t alert);
AlertState_t SensorData_GetTempAlert(void);
