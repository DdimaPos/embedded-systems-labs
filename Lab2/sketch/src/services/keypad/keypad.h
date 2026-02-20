#ifndef KEYPAD_SERVICE_H
#define KEYPAD_SERVICE_H

#include <stdio.h>

void srvKeypadSetup();
int srvKeypadGetKey(FILE *stream);

#endif
