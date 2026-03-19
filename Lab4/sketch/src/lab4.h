#ifndef LAB4_H
#define LAB4_H

#define SHORT_PRESS_THRESHOLD 500

#define TASK1_REC 10
#define TASK1_OFF 1

#define TASK2_REC 50
#define TASK2_OFF 5

#define TASK3_REC 10000
#define TASK3_OFF 10000

#define TASK1_STACK 2048
#define TASK2_STACK 2048
#define TASK3_STACK 4096

#define TASK1_PRIORITY 3
#define TASK2_PRIORITY 2
#define TASK3_PRIORITY 1

void lab4Setup();
void lab4Loop();

#endif
