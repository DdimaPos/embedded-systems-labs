#ifndef SCHEDULER_SERVICE_H
#define SCHEDULER_SERVICE_H

typedef void (*TaskFunc)(void);

typedef struct {
  TaskFunc func;
  int recurrence;
  int offset;
  int counter;
} TaskContext;

void srvSchedulerSetup(TaskContext *tasks, int numTasks);

void srvSchedulerTick();

#endif
