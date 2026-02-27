#include "lab2-1.h"
#include "drivers/button/button.h"
#include "drivers/led/led.h"
#include "services/scheduler/scheduler.h"
#include "services/serial/serial.h"
#include "timer-api.h"
#include "timer_setup.h"

#include <stdio.h>

TaskContext appTasks[NUM_TASKS] = {{task1ButtonDetect, TASK1_REC, TASK1_OFF, 0},
                                   {task2Statistics, TASK2_REC, TASK2_OFF, 0},
                                   {task3Report, TASK3_REC, TASK3_OFF, 0}};

unsigned long systemTick = 0;

unsigned long lastPressDuration = 0;
bool newPressDetected = false;
bool lastPressWasShort = false;

bool buttonPrevState = false;
unsigned long pressStartTick = 0;

int totalPresses = 0;
int shortPresses = 0;
int longPresses = 0;
unsigned long shortDurationSum = 0;
unsigned long longDurationSum = 0;

int yellowBlinksRemaining = 0;
bool yellowIsOn = false;

bool reportReady = false;

int rptTotalPresses = 0;
int rptShortPresses = 0;
int rptLongPresses = 0;
unsigned long rptShortDurationSum = 0;
unsigned long rptLongDurationSum = 0;

void task1ButtonDetect() {
  bool pressed = buttonIsPressed();

  if (pressed && !buttonPrevState) {
    pressStartTick = systemTick;
  } else if (!pressed && buttonPrevState) {
    lastPressDuration = systemTick - pressStartTick;

    if (lastPressDuration < SHORT_PRESS_THRESHOLD) {
      lastPressWasShort = true;
      ledGreenOn();
      ledRedOff();
    } else {
      lastPressWasShort = false;
      ledRedOn();
      ledGreenOff();
    }

    newPressDetected = true;
  }

  buttonPrevState = pressed;
}

void task2Statistics() {
  if (newPressDetected) {
    newPressDetected = false;
    totalPresses++;

    if (lastPressWasShort) {
      shortPresses++;
      shortDurationSum += lastPressDuration;
      yellowBlinksRemaining = 5 * 2;
    } else {
      longPresses++;
      longDurationSum += lastPressDuration;
      yellowBlinksRemaining = 10 * 2;
    }

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
  rptTotalPresses = totalPresses;
  rptShortPresses = shortPresses;
  rptLongPresses = longPresses;
  rptShortDurationSum = shortDurationSum;
  rptLongDurationSum = longDurationSum;

  totalPresses = 0;
  shortPresses = 0;
  longPresses = 0;
  shortDurationSum = 0;
  longDurationSum = 0;

  reportReady = true;
}

void timer_handle_interrupts(int timer) {
  systemTick++;
  srvSchedulerTick();
}

void lab21Setup() {
  setupGreenLed();
  setupRedLed();
  setupYellowLed();
  buttonSetup();
  srvSerialSetup();
  srvSchedulerSetup(appTasks, NUM_TASKS);
  timer_init_ISR_1KHz(TIMER_DEFAULT);

  printf("Lab 3.1 - Button Press Monitor\n");
  printf("Reports every 10 seconds\n");
}

void lab21Loop() {
  if (reportReady) {
    reportReady = false;

    printf("--- Report ---\n");
    printf("Total presses: %d\n", rptTotalPresses);
    printf("Short (<500ms): %d\n", rptShortPresses);
    printf("Long  (>=500ms): %d\n", rptLongPresses);

    if (rptTotalPresses > 0) {
      unsigned long totalDur = rptShortDurationSum + rptLongDurationSum;
      unsigned long avgDur = totalDur / rptTotalPresses;
      printf("Avg duration: %lu ms\n", avgDur);
    } else {
      printf("Avg duration: N/A\n");
    }

    printf("--------------\n");
  }
}
