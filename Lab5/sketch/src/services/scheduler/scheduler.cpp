#include "scheduler.h"
#include <stdio.h>

typedef struct {
  TaskFunc func;
  uint32_t periodMs;
  uint32_t offsetMs;
} TaskWrapper;

static void periodicTaskRunner(void *pvParameters) {
  TaskWrapper *ctx = (TaskWrapper *)pvParameters;

  vTaskDelay(pdMS_TO_TICKS(ctx->offsetMs));

  TickType_t xLastWakeTime = xTaskGetTickCount();
  for (;;) {
    ctx->func();
    vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(ctx->periodMs));
  }
}

#define MAX_TASKS 8
static TaskWrapper wrappers[MAX_TASKS];

void srvSchedulerSetup(TaskDef *tasks, int numTasks) {
  for (int i = 0; i < numTasks && i < MAX_TASKS; i++) {
    wrappers[i].func = tasks[i].func;
    wrappers[i].periodMs = tasks[i].periodMs;
    wrappers[i].offsetMs = tasks[i].offsetMs;

    BaseType_t result =
      xTaskCreate(periodicTaskRunner, tasks[i].name, tasks[i].stackSize,
                  &wrappers[i], tasks[i].priority, NULL);

    if (result != pdPASS) {
      printf("Failed to create task: %s\n", tasks[i].name);
    }
  }
}
