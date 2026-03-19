#include "SensorData.h"

SemaphoreHandle_t xSemNewData  = NULL;
SemaphoreHandle_t xSensorMutex = NULL;

static SensorId_t  gActiveSensor = SENSOR_DHT11;
static float       gRawTemp      = 20.0f;
static float       gFilteredTemp = 20.0f;
static float       gAveragedTemp = 20.0f;
static int         gTempValid    = 0;
static AlertState_t gTempAlert   = ALERT_OFF;

void SensorData_Init(void)
{
    xSemNewData  = xSemaphoreCreateBinary();
    xSensorMutex = xSemaphoreCreateMutex();
}

/* ── Sensor selection ── */
void SensorData_SetActiveSensor(SensorId_t id)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    gActiveSensor = id;
    xSemaphoreGive(xSensorMutex);
}

SensorId_t SensorData_GetActiveSensor(void)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    SensorId_t val = gActiveSensor;
    xSemaphoreGive(xSensorMutex);
    return val;
}

/* ── Raw temperature ── */
void SensorData_SetRawTemp(float val)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    gRawTemp = val;
    xSemaphoreGive(xSensorMutex);
}

float SensorData_GetRawTemp(void)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    float val = gRawTemp;
    xSemaphoreGive(xSensorMutex);
    return val;
}

/* ── Filtered temperature (after median) ── */
void SensorData_SetFilteredTemp(float val)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    gFilteredTemp = val;
    xSemaphoreGive(xSensorMutex);
}

float SensorData_GetFilteredTemp(void)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    float val = gFilteredTemp;
    xSemaphoreGive(xSensorMutex);
    return val;
}

/* ── Averaged temperature (after EMA) ── */
void SensorData_SetAveragedTemp(float val)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    gAveragedTemp = val;
    xSemaphoreGive(xSensorMutex);
}

float SensorData_GetAveragedTemp(void)
{
    xSemaphoreTake(xSensorMutex, portMAX_DELAY);
    float val = gAveragedTemp;
    xSemaphoreGive(xSensorMutex);
    return val;
}

/* ── Validity ── */
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

/* ── Alert ── */
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
