#ifndef LAB21_H
#define LAB21_H

void lab21Setup();

#define SRV_OS_SYS_TICK 1  // 1 ms (1 KHr)
#define TIME_SEC 1000
#define SHORT_PRESS_THRESHOLD 500

#define TASK1_REC 10
#define TASK1_OFF 1
#define TASK2_REC 50
#define TASK2_OFF 5
#define TASK3_REC (10 * TIME_SEC)
#define TASK3_OFF (10 * TIME_SEC)

void task1ButtonDetect();
void task2Statistics();
void task3Report();
void lab21Loop();

#define NUM_TASKS 3

#endif
