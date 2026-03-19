#include "TaskAcquisition.h"
#include "TempSensor.h"
#include "SensorData.h"

#define ACQUISITION_PERIOD_MS 100  // Matches DS18B20 ~94ms conversion

void TaskAcquisition_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        // Read temperature from DS18B20
        float temp = TempSensor_Read();
        SensorData_SetTemperature(temp);
        SensorData_SetTempValid(TempSensor_IsValid());

        // Signal conditioning task that new data is available
        xSemaphoreGive(xSemNewData);

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ACQUISITION_PERIOD_MS));
    }
}
