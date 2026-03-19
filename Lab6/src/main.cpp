
#include "SerialIO.h"
#include "TempSensor.h"
#include "LedDriver.h"
#include "LcdDisplay.h"
#include "OledDisplay.h"
#include "SensorData.h"
#include "TaskAcquisition.h"
#include "TaskConditioning.h"
#include "TaskReporter.h"

void setup()
{
    /* ── Driver / ECAL initialisation ── */
    SerialIoInit();
    TempSensor_Init();   // DS18B20 on GPIO4, 9-bit non-blocking
    LedDriver_Init();
    LcdDisplay_Init();   // LCD 16x2 I2C (0x27, SDA=21, SCL=22)
    OledDisplay_Init();  // OLED 128x64 SSD1306 I2C (0x3C)

    /* ── Service layer initialisation ── */
    SensorData_Init();   // semaphore + mutex
    SensorData_SetActiveSensor(SENSOR_DHT11);

    printf("\n======================================\n");
    printf("  Lab 6 - Analog Signal Acquisition\n");
    printf("======================================\n");
    printf("Sensor    : DHT11 (digital) GPIO%d\n", TEMP_SENSOR_PIN);
    printf("Acq.period: %d ms\n", ACQUISITION_PERIOD_MS);
    printf("Pipeline  : Saturation -> Median(%d) -> EMA(a=%.2f)\n",
           MEDIAN_WINDOW, EMA_ALPHA);
    printf("Thresholds: ON >= %.1fC, OFF <= %.1fC\n",
           TEMP_THRESH_HIGH, TEMP_THRESH_LOW);
    printf("Display   : OLED 128x64 SSD1306 + LCD 16x2 + Serial\n");
    printf("======================================\n\n");

    LcdDisplay_PrintAt(0, 0, "Lab6 SigAcquire");
    LcdDisplay_PrintAt(0, 1, "Starting...");

    OledDisplay_Clear();
    OledDisplay_SetTextSize(1);
    OledDisplay_PrintAt(0, 0, "Lab6 SigAcquire");
    OledDisplay_PrintAt(0, 16, "Starting...");
    OledDisplay_Update();

    vTaskDelay(pdMS_TO_TICKS(1000));

    /* ── FreeRTOS tasks (priorities: Acq=3, Cond=2, Reporter=1) ── */
    xTaskCreate(TaskAcquisition_Task,  "Acquisition",  2048, NULL, 3, NULL);
    xTaskCreate(TaskConditioning_Task, "Conditioning", 2048, NULL, 2, NULL);
    xTaskCreate(TaskReporter_Task,     "Reporter",     4096, NULL, 1, NULL);
}

void loop()
{
    vTaskDelay(portMAX_DELAY);
}
