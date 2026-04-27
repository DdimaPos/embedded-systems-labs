/*
 * TaskAcquisition – reads the LDR every ACQUISITION_PERIOD_MS.
 *
 * Period justification: the LDR has no minimum sampling constraint (unlike
 * the DHT11's 2 s). 100 ms provides fresh data to TaskControl while keeping
 * CPU load negligible.
 */
#include "TaskAcquisition.h"
#include "SystemData.h"
#include "LdrDriver.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

void TaskAcquisition_Task(void *pvParameters)
{
    (void)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        float light = LdrDriver_ReadPercent();
        SystemData_SetLightLevel(light);
        SystemData_SetFault(0);  /* LDR always produces a value; no validity concern */

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ACQUISITION_PERIOD_MS));
    }
}
