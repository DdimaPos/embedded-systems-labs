#include "TaskAcquisition.h"
#include "TempSensor.h"
#include "SensorData.h"

/*
 * TaskAcquisition – reads the currently selected sensor at a configurable
 * period (ACQUISITION_PERIOD_MS, 20-100 ms).  Exposes the raw value via
 * SensorData_SetRawTemp() and signals the conditioning task.
 */
void TaskAcquisition_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        SensorId_t activeSensor = SensorData_GetActiveSensor();

        float rawTemp = 0.0f;
        int   valid   = 0;

        switch (activeSensor) {
            case SENSOR_DHT11:
            default:
                rawTemp = TempSensor_Read();
                valid   = TempSensor_IsValid();
                break;
            /* Future sensors: case SENSOR_LM35: ... break; */
        }

        SensorData_SetRawTemp(rawTemp);
        SensorData_SetTempValid(valid);

        /* Signal conditioning task that fresh raw data is available */
        xSemaphoreGive(xSemNewData);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ACQUISITION_PERIOD_MS));
    }
}
