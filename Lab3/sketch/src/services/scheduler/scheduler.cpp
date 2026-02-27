#include "scheduler.h"

static TaskContext *schedulerTasks = 0;
static int schedulerNumTasks = 0;

void srvSchedulerSetup(TaskContext *tasks, int numTasks) {
  schedulerTasks = tasks;
  schedulerNumTasks = numTasks;
  for (int i = 0; i < numTasks; i++) {
    tasks[i].counter = tasks[i].offset;
  }
}

void srvSchedulerTick() {
  if (!schedulerTasks)
    return;

  for (int i = 0; i < schedulerNumTasks; i++) {
    if (--schedulerTasks[i].counter <= 0) {
      schedulerTasks[i].counter = schedulerTasks[i].recurrence;
      schedulerTasks[i].func();
      break;
    }
  }
}
