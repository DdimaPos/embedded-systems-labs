
#include "SerialIO.h"
#include "TempSensor.h"
#include "LedDriver.h"
#include "LcdDisplay.h"
#include "SensorData.h"
#include "TaskAcquisition.h"
#include "TaskConditioning.h"
#include "TaskReporter.h"

void setup()
{
    SerialIoInit();
    TempSensor_Init();   // DS18B20 on GPIO4, 9-bit non-blocking
    LedDriver_Init();
    LcdDisplay_Init();   // LCD 16x2 I2C (0x27, SDA=21, SCL=22)
    SensorData_Init();   // semaphore + mutex

    printf("\n======================================\n");
    printf("  Lab 5 - Temperature Monitor\n");
    printf("======================================\n");
    printf("Sensor    : DS18B20 (digital) GPIO%d\n", TEMP_SENSOR_PIN);
    printf("Thresh    : ON >= %.1fC, OFF <= %.1fC\n", TEMP_THRESH_HIGH, TEMP_THRESH_LOW);
    printf("Debounce  : %d x 100ms = %dms\n", DEBOUNCE_COUNT, DEBOUNCE_COUNT * 100);
    printf("Display   : LCD 16x2 I2C + Serial\n");
    printf("======================================\n\n");

    LcdDisplay_PrintAt(0, 0, "Lab5 TempMonitor");
    LcdDisplay_PrintAt(0, 1, "Starting...");
    vTaskDelay(pdMS_TO_TICKS(1000));

    // Task priorities: Acquisition=3 (highest), Conditioning=2, Reporter=1
    xTaskCreate(TaskAcquisition_Task,  "Acquisition",  2048, NULL, 3, NULL);
    xTaskCreate(TaskConditioning_Task, "Conditioning", 2048, NULL, 2, NULL);
    xTaskCreate(TaskReporter_Task,     "Reporter",     4096, NULL, 1, NULL);
}

void loop()
{
    vTaskDelay(portMAX_DELAY);
}
