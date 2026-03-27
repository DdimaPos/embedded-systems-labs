#include "TaskCommand.h"
#include "ActuatorData.h"
#include <Arduino.h>

#define CMD_BUF_SIZE 16

static char cmdBuf[CMD_BUF_SIZE];
static uint8_t cmdIdx = 0;

static void ProcessCommand(const char *cmd)
{
    /* Make uppercase copy for case-insensitive parsing */
    char upper[CMD_BUF_SIZE];
    uint8_t i = 0;
    while (cmd[i] && i < CMD_BUF_SIZE - 1) {
        upper[i] = toupper(cmd[i]);
        i++;
    }
    upper[i] = '\0';

    if (upper[0] == 'S' && upper[1] >= '0' && upper[1] <= '9') {
        /* S<0-100> : Set speed */
        int speed = atoi(&upper[1]);
        if (speed < SPEED_MIN || speed > SPEED_MAX) {
            printf("[CMD] ERROR: speed %d out of range [%d-%d], clamping\n",
                   speed, SPEED_MIN, SPEED_MAX);
            if (speed < SPEED_MIN) speed = SPEED_MIN;
            if (speed > SPEED_MAX) speed = SPEED_MAX;
        }
        ActuatorData_SetEmergencyStop(0);
        ActuatorData_SetTargetSpeed((int16_t)speed);
        xSemaphoreGive(xSemNewCommand);
        printf("[CMD] Speed target -> %d%%\n", speed);

    } else if (strcmp(upper, "DF") == 0) {
        ActuatorData_SetDirection(DIR_FORWARD);
        printf("[CMD] Direction -> FORWARD\n");

    } else if (strcmp(upper, "DR") == 0) {
        ActuatorData_SetDirection(DIR_REVERSE);
        printf("[CMD] Direction -> REVERSE\n");

    } else if (strcmp(upper, "X") == 0) {
        ActuatorData_SetEmergencyStop(1);
        ActuatorData_SetTargetSpeed(0);
        xSemaphoreGive(xSemNewCommand);
        printf("[CMD] EMERGENCY STOP\n");

    } else if (strcmp(upper, "RON") == 0) {
        ActuatorData_SetRelayTarget(RELAY_ON);
        printf("[CMD] Relay -> ON\n");

    } else if (strcmp(upper, "ROFF") == 0) {
        ActuatorData_SetRelayTarget(RELAY_OFF);
        printf("[CMD] Relay -> OFF\n");

    } else if (strcmp(upper, "?") == 0) {
        /* Status query - print full report */
        float target = (float)ActuatorData_GetTargetSpeed();
        float cond   = ActuatorData_GetConditionedSpeed();
        float curr   = ActuatorData_GetCurrentSpeed();
        Direction_t  dir   = ActuatorData_GetDirection();
        MotorState_t mst   = ActuatorData_GetMotorState();
        RelayState_t rst   = ActuatorData_GetRelayState();
        AlertState_t alert = ActuatorData_GetAlert();

        printf("\n===== STATUS QUERY =====\n");
        printf("Target speed : %.0f%%\n", target);
        printf("Conditioned  : %.1f%%\n", cond);
        printf("Current      : %.1f%%\n", curr);
        printf("Direction    : %s\n", dir == DIR_FORWARD ? "FORWARD" : "REVERSE");
        printf("Motor state  : %s\n",
               mst == MOTOR_IDLE ? "IDLE" :
               mst == MOTOR_RAMP_UP ? "RAMP_UP" :
               mst == MOTOR_RUNNING ? "RUNNING" : "RAMP_DOWN");
        printf("Relay        : %s\n", rst == RELAY_ON ? "ON" : "OFF");
        printf("Alert        : %s\n",
               alert == ALERT_NONE ? "NONE" :
               alert == ALERT_LIMIT_REACHED ? "LIMIT_REACHED" : "OVERLOAD");
        printf("========================\n\n");

    } else {
        printf("[CMD] ERROR: unknown command '%s'\n", cmd);
        printf("[CMD] Usage: S<0-100> DF DR X RON ROFF ?\n");
    }
}

void TaskCommand_Task(void *pvParameters)
{
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        /* Non-blocking serial read */
        while (Serial.available() > 0) {
            char c = (char)Serial.read();

            if (c == '\n' || c == '\r') {
                if (cmdIdx > 0) {
                    cmdBuf[cmdIdx] = '\0';
                    ProcessCommand(cmdBuf);
                    cmdIdx = 0;
                }
            } else if (cmdIdx < CMD_BUF_SIZE - 1) {
                cmdBuf[cmdIdx++] = c;
            }
        }

        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(COMMAND_PERIOD_MS));
    }
}
