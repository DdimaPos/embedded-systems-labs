#include "lab5.h"
#include "drivers/dht11/dht11.h"
#include "drivers/led/led.h"
#include "services/scheduler/scheduler.h"
#include "services/serial/serial.h"
#include "services/temperature/temperature.h"

#include <stdio.h>

// ── Task 1: Sensor Acquisition ────────────────────────────────────────────────
// Reads raw temperature and humidity from the DHT11 driver and forwards
// the result to the temperature service. No processing is done here.
static void taskAcquisition() {
  float temperature = 0.0f;
  float humidity    = 0.0f;
  bool  valid       = dht11Read(&temperature, &humidity);
  tempServiceUpdateRaw(temperature, humidity, valid);
}

// ── Task 2: Signal Conditioning ───────────────────────────────────────────────
// Applies the hysteresis threshold filter and anti-bouncing logic via the
// temperature service, then reflects the resulting state on the LEDs:
//   GREEN  → temperature is within normal range
//   YELLOW → debounce in progress (state transition being confirmed)
//   RED    → alert confirmed (temperature above threshold)
static void taskCondition() {
  tempServiceCondition();

  TempAlertState state = tempGetAlertState();

  switch (state) {
    case TEMP_STATE_NORMAL:
      ledGreenOn();
      ledYellowOff();
      ledRedOff();
      break;
    case TEMP_STATE_DEBOUNCING:
      ledGreenOff();
      ledYellowOn();
      ledRedOff();
      break;
    case TEMP_STATE_ALERT:
      ledGreenOff();
      ledYellowOff();
      ledRedOn();
      break;
  }
}

// ── Task 3: Display & Reporting ───────────────────────────────────────────────
// Prints a structured report with all intermediate conditioning values,
// the current system state, and any active alerts.
static void taskReport() {
  float          t     = tempGetRawTemperature();
  float          h     = tempGetRawHumidity();
  bool           valid = tempGetReadValid();
  TempAlertState state = tempGetAlertState();
  int            dbc   = tempGetDebounceCounter();

  const char *stateStr;
  switch (state) {
    case TEMP_STATE_NORMAL:     stateStr = "NORMAL";       break;
    case TEMP_STATE_DEBOUNCING: stateStr = "DEBOUNCING";   break;
    case TEMP_STATE_ALERT:      stateStr = "*** ALERT ***"; break;
    default:                    stateStr = "UNKNOWN";
  }

  printf("┌─────────────────────────────────────┐\n");
  printf("│     Temperature Monitor  Report     │\n");
  printf("├─────────────────────────────────────┤\n");
  printf("│ Sensor read    : %-18s │\n", valid ? "OK" : "FAIL (no data)");
  printf("│ Raw temp       : %+6.1f C              │\n", t);
  printf("│ Raw humidity   : %5.1f %%              │\n", h);
  printf("├─────────────────────────────────────┤\n");
  printf("│ Threshold HIGH : %5.1f C              │\n", (float)TEMP_THRESHOLD_HIGH);
  printf("│ Threshold LOW  : %5.1f C              │\n", (float)TEMP_THRESHOLD_LOW);
  printf("│ Debounce cnt   : %2d / %-2d             │\n", dbc < 0 ? -dbc : dbc,
         TEMP_DEBOUNCE_COUNT);
  printf("├─────────────────────────────────────┤\n");
  printf("│ State          : %-18s │\n", stateStr);
  printf("└─────────────────────────────────────┘\n");
}

// ── Setup & Loop ──────────────────────────────────────────────────────────────

void lab5Setup() {
  srvSerialSetup();

  setupGreenLed();
  setupRedLed();
  setupYellowLed();

  dht11Init(DHT11_PIN);
  tempServiceInit();

  printf("Lab 5 – Binary Signal Acquisition (DHT11 Temperature Monitor)\n");
  printf("Hysteresis band : %.1f – %.1f C  |  Debounce: %d readings @ %d ms\n",
         (float)TEMP_THRESHOLD_LOW, (float)TEMP_THRESHOLD_HIGH,
         TEMP_DEBOUNCE_COUNT, COND_PERIOD_MS);

  static TaskDef tasks[] = {
    {"Task_Acq",    taskAcquisition, ACQ_PERIOD_MS,  ACQ_OFFSET_MS,  ACQ_STACK_SIZE,  ACQ_PRIORITY},
    {"Task_Cond",   taskCondition,   COND_PERIOD_MS, COND_OFFSET_MS, COND_STACK_SIZE, COND_PRIORITY},
    {"Task_Report", taskReport,      RPT_PERIOD_MS,  RPT_OFFSET_MS,  RPT_STACK_SIZE,  RPT_PRIORITY},
  };

  srvSchedulerSetup(tasks, 3);
}

void lab5Loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}
