#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

/* ECAL */
#include "LdrDriver.h"
#include "LedDriver.h"
#include "OledDisplay.h"
#include "SerialIO.h"

/* SRV */
#include "SystemData.h"
#include "TaskAcquisition.h"
#include "TaskCommand.h"
#include "TaskControl.h"
#include "TaskReporter.h"

void setup()
{
    /* ── Driver / ECAL initialisation ───────────────────────────────────── */
    SerialIoInit();       /* must be first: enables printf/getchar via UART VFS */
    LdrDriver_Init();
    LedDriver_Init();
    OledDisplay_Init();

    /* ── Service layer initialisation ───────────────────────────────────── */
    SystemData_Init();    /* creates xSystemMutex — must run before tasks start */

    /* ── Startup banner ─────────────────────────────────────────────────── */
    printf("=====================================\n");
    printf("  Lab9 - ON/OFF Light Control        \n");
    printf("=====================================\n");
    printf(" LDR sensor    : GPIO %d  (ADC1_CH6)\n",  LDR_PIN);
    printf(" LED actuator  : GPIO %d  (LEDC CH%d, %d Hz)\n",
           LED_PIN, LED_LEDC_CHANNEL, LED_LEDC_FREQ);
    printf(" OLED SDA/SCL  : GPIO %d / %d\n", OLED_SDA_PIN, OLED_SCL_PIN);
    printf("-------------------------------------\n");
    printf(" Default SP    : %.1f %%\n", DEFAULT_SETPOINT);
    printf(" Default HY    : %.1f %%  band [%.1f .. %.1f]\n",
           DEFAULT_HYSTERESIS,
           DEFAULT_SETPOINT - DEFAULT_HYSTERESIS * 0.5f,
           DEFAULT_SETPOINT + DEFAULT_HYSTERESIS * 0.5f);
    printf("-------------------------------------\n");
    printf(" Task periods:\n");
    printf("   Acquisition  %4u ms\n", ACQUISITION_PERIOD_MS);
    printf("   Control      %4u ms\n", CONTROL_PERIOD_MS);
    printf("   Command      %4u ms\n", COMMAND_PERIOD_MS);
    printf("   Reporter     %4u ms\n", REPORTER_PERIOD_MS);
    printf("=====================================\n\n");

    /* ── Splash screen ──────────────────────────────────────────────────── */
    char splash[32];
    snprintf(splash, sizeof(splash), "SP=%.1f  HY=%.1f %%",
             DEFAULT_SETPOINT, DEFAULT_HYSTERESIS);
    OledDisplay_Clear();
    OledDisplay_SetTextSize(1);
    OledDisplay_PrintAt(10, 10, "ON-OFF LIGHT CTRL");
    OledDisplay_PrintAt(22, 28, "Initialising...");
    OledDisplay_PrintAt(4,  44, splash);
    OledDisplay_Update();

    vTaskDelay(pdMS_TO_TICKS(1000));

    /* ── FreeRTOS task creation ─────────────────────────────────────────── */
    /*   Priority 3 = highest: sensor read must not be starved               */
    /*   Priority 2: control and command run at same level, interleaved      */
    /*   Priority 1 = lowest: reporter only runs when others are idle        */
    xTaskCreate(TaskAcquisition_Task, "Acquisition", 2048, NULL, 3, NULL);
    xTaskCreate(TaskControl_Task,     "Control",     2048, NULL, 2, NULL);
    xTaskCreate(TaskCommand_Task,     "Command",     4096, NULL, 2, NULL);
    xTaskCreate(TaskReporter_Task,    "Reporter",    4096, NULL, 1, NULL);
}

void loop()
{
    /* All work is done inside FreeRTOS tasks */
    vTaskDelay(portMAX_DELAY);
}
