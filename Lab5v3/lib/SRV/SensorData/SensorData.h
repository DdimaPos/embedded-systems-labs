#pragma once
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

// Thresholds (hysteresis)
#define TEMP_THRESH_HIGH   26.0f
#define TEMP_THRESH_LOW    24.0f

#define DEBOUNCE_COUNT     3   // 3 x 100ms = 300ms debounce

// Alert state
typedef enum {
    ALERT_OFF = 0,
    ALERT_ON  = 1
} AlertState_t;

// Synchronization primitives
extern SemaphoreHandle_t xSemNewData;
extern SemaphoreHandle_t xSensorMutex;

void  SensorData_Init(void);

// Temperature
void         SensorData_SetTemperature(float val);
float        SensorData_GetTemperature(void);
void         SensorData_SetTempValid(int valid);
int          SensorData_IsTempValid(void);
void         SensorData_SetTempAlert(AlertState_t alert);
AlertState_t SensorData_GetTempAlert(void);
void         SensorData_SetTempDebounce(uint8_t val);
uint8_t      SensorData_GetTempDebounce(void);
