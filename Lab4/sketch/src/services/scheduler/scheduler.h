#ifndef SCHEDULER_SERVICE_H
#define SCHEDULER_SERVICE_H

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

typedef void (*TaskFunc)(void);

typedef struct {
  const char *name;
  TaskFunc func;
  uint32_t periodMs;
  uint32_t offsetMs;
  uint32_t stackSize;
  UBaseType_t priority;
} TaskDef;

void srvSchedulerSetup(TaskDef *tasks, int numTasks);

#endif
