#ifndef TEMPERATURE_SERVICE_H
#define TEMPERATURE_SERVICE_H

#include <stdbool.h>

// Hysteresis band: alert triggers above HIGH, clears below LOW (25°C ± 1°C)
#define TEMP_THRESHOLD_HIGH  26.0f
#define TEMP_THRESHOLD_LOW   24.0f

// Number of consecutive condition-task cycles that must confirm a state
// change before the alert transitions (anti-bouncing interval).
// At COND_PERIOD_MS=200ms this gives a 600ms minimum confirmation window.
#define TEMP_DEBOUNCE_COUNT  3

typedef enum {
  TEMP_STATE_NORMAL      = 0,
  TEMP_STATE_DEBOUNCING  = 1,  // transitioning, not yet confirmed
  TEMP_STATE_ALERT       = 2
} TempAlertState;

void           tempServiceInit();

// Called by the acquisition task with the latest raw sensor values.
void           tempServiceUpdateRaw(float temperature, float humidity, bool valid);

// Called by the conditioning task: applies hysteresis + debounce and updates
// the internal alert state. Must be called at a regular, configurable rate.
void           tempServiceCondition();

float          tempGetRawTemperature();
float          tempGetRawHumidity();
bool           tempGetReadValid();
TempAlertState tempGetAlertState();
bool           tempIsAlert();
int            tempGetDebounceCounter();

#endif
