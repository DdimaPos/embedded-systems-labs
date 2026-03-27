#include "SerialIO.h"
#include "MotorDriver.h"
#include "RelayDriver.h"
#include "LedDriver.h"
#include "OledDisplay.h"
#include "ActuatorData.h"
#include "TaskCommand.h"
#include "TaskConditioning.h"
#include "TaskActuator.h"
#include "TaskReporter.h"

void setup()
{
    /* ── Driver / ECAL initialisation ── */
    SerialIoInit();
    MotorDriver_Init();
    RelayDriver_Init();
    LedDriver_Init();
    OledDisplay_Init();

    /* ── Service layer initialisation ── */
    ActuatorData_Init();

    printf("\n==========================================\n");
    printf("  Lab 7 - Actuator Control System (Var C)\n");
    printf("==========================================\n");
    printf("Analog  : L298 PWM on GPIO%d (LED test)\n", MOTOR_ENA_PIN);
    printf("          IN1=GPIO%d  IN2=GPIO%d\n", MOTOR_IN1_PIN, MOTOR_IN2_PIN);
    printf("Binary  : Relay on GPIO%d (active-HIGH)\n", RELAY_PIN);
    printf("Pipeline: Sat -> Median(%d) -> EMA(a=%.2f) -> Ramp(%d/cyc)\n",
           MEDIAN_WINDOW, EMA_ALPHA, RAMP_STEP);
    printf("Overload: >= %d%%\n", OVERLOAD_THRESHOLD);
    printf("Commands: S<0-100> DF DR X RON ROFF ?\n");
    printf("==========================================\n\n");

    OledDisplay_Clear();
    OledDisplay_SetTextSize(1);
    OledDisplay_PrintAt(4, 0,  "-- Actuator Ctrl --");
    OledDisplay_PrintAt(0, 32, "Starting...");
    OledDisplay_Update();

    vTaskDelay(pdMS_TO_TICKS(1000));

    /* ── FreeRTOS tasks ── */
    xTaskCreate(TaskCommand_Task,      "Command",      2048, NULL, 3, NULL);
    xTaskCreate(TaskConditioning_Task, "Conditioning", 2048, NULL, 2, NULL);
    xTaskCreate(TaskActuator_Task,     "Actuator",     2048, NULL, 2, NULL);
    xTaskCreate(TaskReporter_Task,     "Reporter",     4096, NULL, 1, NULL);
}

void loop()
{
    vTaskDelay(portMAX_DELAY);
}
