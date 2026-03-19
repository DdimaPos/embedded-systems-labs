#include "lab4.h"
#include "drivers/button/button.h"
#include "drivers/led/led.h"
#include "services/scheduler/scheduler.h"
#include "services/serial/serial.h"
#include "services/sync/sync.h"

#include <stdio.h>

static unsigned long lastPressDuration = 0;
static bool lastPressWasShort = false;

static int totalPresses = 0;
static int shortPresses = 0;
static int longPresses = 0;
static unsigned long shortDurationSum = 0;
static unsigned long longDurationSum = 0;

static bool buttonPrevState = false;
static unsigned long pressStartMs = 0;

void task1ButtonDetect() {
  bool pressed = buttonIsPressed();

  if (pressed && !buttonPrevState) {
    pressStartMs = millis();
  } else if (!pressed && buttonPrevState) {
    unsigned long duration = millis() - pressStartMs;
    bool isShort = (duration < SHORT_PRESS_THRESHOLD);

    syncMutexTake();
    lastPressDuration = duration;
    lastPressWasShort = isShort;
    syncMutexGive();

    if (isShort) {
      ledGreenOn();
      ledRedOff();
    } else {
      ledRedOn();
      ledGreenOff();
    }

    syncSemaphoreGive();
  }

  buttonPrevState = pressed;
}

static int yellowBlinksRemaining = 0;
static bool yellowIsOn = false;

void task2Statistics() {
  if (syncSemaphoreTake(0)) {
    syncMutexTake();
    unsigned long duration = lastPressDuration;
    bool isShort = lastPressWasShort;
    syncMutexGive();

    syncMutexTake();
    totalPresses++;
    if (isShort) {
      shortPresses++;
      shortDurationSum += duration;
      yellowBlinksRemaining = 5 * 2;
    } else {
      longPresses++;
      longDurationSum += duration;
      yellowBlinksRemaining = 10 * 2;
    }
    syncMutexGive();

    yellowIsOn = false;
  }

  if (yellowBlinksRemaining > 0) {
    yellowBlinksRemaining--;
    yellowIsOn = !yellowIsOn;
    if (yellowIsOn) {
      ledYellowOn();
    } else {
      ledYellowOff();
    }
  } else if (yellowIsOn) {
    ledYellowOff();
    yellowIsOn = false;
  }
}

void task3Report() {
  syncMutexTake();
  int rptTotal = totalPresses;
  int rptShort = shortPresses;
  int rptLong = longPresses;
  unsigned long rptShortSum = shortDurationSum;
  unsigned long rptLongSum = longDurationSum;

  totalPresses = 0;
  shortPresses = 0;
  longPresses = 0;
  shortDurationSum = 0;
  longDurationSum = 0;
  syncMutexGive();

  printf("--- Report ---\n");
  printf("Total presses: %d\n", rptTotal);
  printf("Short (<500ms): %d\n", rptShort);
  printf("Long  (>=500ms): %d\n", rptLong);

  if (rptTotal > 0) {
    unsigned long avgDur = (rptShortSum + rptLongSum) / rptTotal;
    printf("Avg duration: %lu ms\n", avgDur);
  } else {
    printf("Avg duration: N/A\n");
  }

  printf("--------------\n");
}

void lab4Setup() {
  setupGreenLed();
  setupRedLed();
  setupYellowLed();
  buttonSetup();
  srvSerialSetup();
  syncSetup();

  printf("Lab 4 - FreeRTOS Button Press Monitor\n");
  printf("Reports every 10 seconds\n");

  TaskDef tasks[] = {
      {"Task1_Button", task1ButtonDetect, TASK1_REC, TASK1_OFF, TASK1_STACK,
       TASK1_PRIORITY},
      {"Task2_Stats", task2Statistics, TASK2_REC, TASK2_OFF, TASK2_STACK,
       TASK2_PRIORITY},
      {"Task3_Report", task3Report, TASK3_REC, TASK3_OFF, TASK3_STACK,
       TASK3_PRIORITY},
  };

  srvSchedulerSetup(tasks, 3);
}

void lab4Loop() { vTaskDelay(pdMS_TO_TICKS(1000)); }
