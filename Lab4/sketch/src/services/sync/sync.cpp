#include "sync.h"
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

static SemaphoreHandle_t pressSemaphore = NULL;
static SemaphoreHandle_t statsMutex = NULL;

void syncSetup() {
  pressSemaphore = xSemaphoreCreateBinary();
  statsMutex = xSemaphoreCreateMutex();
}

void syncSemaphoreGive() {
  xSemaphoreGive(pressSemaphore);
}

bool syncSemaphoreTake(uint32_t timeoutMs) {
  return xSemaphoreTake(pressSemaphore, pdMS_TO_TICKS(timeoutMs)) == pdTRUE;
}

void syncMutexTake() {
  xSemaphoreTake(statsMutex, portMAX_DELAY);
}

void syncMutexGive() {
  xSemaphoreGive(statsMutex);
}
