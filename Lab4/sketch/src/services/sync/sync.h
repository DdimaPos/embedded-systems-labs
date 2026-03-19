#ifndef SYNC_SERVICE_H
#define SYNC_SERVICE_H

#include <stdbool.h>
#include <stdint.h>

void syncSetup();

void syncSemaphoreGive();
bool syncSemaphoreTake(uint32_t timeoutMs);

void syncMutexTake();
void syncMutexGive();

#endif
