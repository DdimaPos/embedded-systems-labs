#include "temperature.h"
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

static float          rawTemperature = 0.0f;
static float          rawHumidity    = 0.0f;
static bool           readValid      = false;
static TempAlertState alertState     = TEMP_STATE_NORMAL;
static int            debounceCount  = 0;

static SemaphoreHandle_t dataMutex = NULL;

void tempServiceInit() {
  dataMutex = xSemaphoreCreateMutex();
}

void tempServiceUpdateRaw(float temperature, float humidity, bool valid) {
  xSemaphoreTake(dataMutex, portMAX_DELAY);
  rawTemperature = temperature;
  rawHumidity    = humidity;
  readValid      = valid;
  xSemaphoreGive(dataMutex);
}

// Hysteresis + anti-bouncing filter.
//
// Transitions:
//   NORMAL      → DEBOUNCING when temp crosses THRESHOLD_HIGH (upward)
//   DEBOUNCING  → ALERT       after DEBOUNCE_COUNT consecutive above-HIGH readings
//   DEBOUNCING  → NORMAL      if temp drops back into the hysteresis band
//   ALERT       → DEBOUNCING  when temp crosses THRESHOLD_LOW (downward)
//   DEBOUNCING  → NORMAL      after DEBOUNCE_COUNT consecutive below-LOW readings
//   DEBOUNCING  → ALERT       if temp climbs back above LOW before count reached
void tempServiceCondition() {
  xSemaphoreTake(dataMutex, portMAX_DELAY);

  if (!readValid) {
    xSemaphoreGive(dataMutex);
    return;
  }

  float t = rawTemperature;

  switch (alertState) {
    case TEMP_STATE_NORMAL:
      if (t > TEMP_THRESHOLD_HIGH) {
        debounceCount++;
        alertState = TEMP_STATE_DEBOUNCING;
      }
      break;

    case TEMP_STATE_DEBOUNCING:
      // Detect which direction we are debouncing
      if (debounceCount > 0) {
        // Rising edge debounce (NORMAL → ALERT)
        if (t > TEMP_THRESHOLD_HIGH) {
          debounceCount++;
          if (debounceCount >= TEMP_DEBOUNCE_COUNT) {
            alertState    = TEMP_STATE_ALERT;
            debounceCount = 0;
          }
        } else if (t <= TEMP_THRESHOLD_HIGH && t >= TEMP_THRESHOLD_LOW) {
          // Back in hysteresis band – reset
          debounceCount = 0;
          alertState    = TEMP_STATE_NORMAL;
        }
      } else {
        // Falling edge debounce (ALERT → NORMAL)
        if (t < TEMP_THRESHOLD_LOW) {
          debounceCount--;
          if (debounceCount <= -TEMP_DEBOUNCE_COUNT) {
            alertState    = TEMP_STATE_NORMAL;
            debounceCount = 0;
          }
        } else if (t >= TEMP_THRESHOLD_LOW) {
          // Climbed back above LOW – stay in ALERT
          debounceCount = 0;
          alertState    = TEMP_STATE_ALERT;
        }
      }
      break;

    case TEMP_STATE_ALERT:
      if (t < TEMP_THRESHOLD_LOW) {
        debounceCount--;
        alertState = TEMP_STATE_DEBOUNCING;
      }
      break;
  }

  xSemaphoreGive(dataMutex);
}

float tempGetRawTemperature() {
  xSemaphoreTake(dataMutex, portMAX_DELAY);
  float v = rawTemperature;
  xSemaphoreGive(dataMutex);
  return v;
}

float tempGetRawHumidity() {
  xSemaphoreTake(dataMutex, portMAX_DELAY);
  float v = rawHumidity;
  xSemaphoreGive(dataMutex);
  return v;
}

bool tempGetReadValid() {
  xSemaphoreTake(dataMutex, portMAX_DELAY);
  bool v = readValid;
  xSemaphoreGive(dataMutex);
  return v;
}

TempAlertState tempGetAlertState() {
  xSemaphoreTake(dataMutex, portMAX_DELAY);
  TempAlertState v = alertState;
  xSemaphoreGive(dataMutex);
  return v;
}

bool tempIsAlert() {
  return tempGetAlertState() == TEMP_STATE_ALERT;
}

int tempGetDebounceCounter() {
  xSemaphoreTake(dataMutex, portMAX_DELAY);
  int v = debounceCount;
  xSemaphoreGive(dataMutex);
  return v;
}
