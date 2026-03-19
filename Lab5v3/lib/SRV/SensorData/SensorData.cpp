#include "SensorData.h"

SemaphoreHandle_t xSemNewData  = NULL;
SemaphoreHandle_t xSensorMutex = NULL;

static float        gTemperature  = 20.0f;
static int          gTempValid    = 0;
static AlertState_t gTempAlert    = ALERT_OFF;
static uint8_t      gTempDebounce = 0;

void SensorData_Init(void)
{
    xSemNewData  = xSemaphoreCreateBinary();
    xSensorMutex = xSemaphoreCreateMutex();
}

void SensorData_SetTemperature(float val)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    gTemperature = val;
    xSemaphoreGive(xSensorMutex);
}

float SensorData_GetTemperature(void)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    float val = gTemperature;
    xSemaphoreGive(xSensorMutex);
    return val;
}

void SensorData_SetTempValid(int valid)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    gTempValid = valid;
    xSemaphoreGive(xSensorMutex);
}

int SensorData_IsTempValid(void)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    int val = gTempValid;
    xSemaphoreGive(xSensorMutex);
    return val;
}

void SensorData_SetTempAlert(AlertState_t alert)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    gTempAlert = alert;
    xSemaphoreGive(xSensorMutex);
}

AlertState_t SensorData_GetTempAlert(void)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    AlertState_t val = gTempAlert;
    xSemaphoreGive(xSensorMutex);
    return val;
}

void SensorData_SetTempDebounce(uint8_t val)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    gTempDebounce = val;
    xSemaphoreGive(xSensorMutex);
}

uint8_t SensorData_GetTempDebounce(void)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    uint8_t val = gTempDebounce;
    xSemaphoreGive(xSensorMutex);
    return val;
}
