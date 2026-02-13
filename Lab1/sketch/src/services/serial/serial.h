#ifndef SERIAL_DRIVER_STDIO_H
#define SERIAL_DRIVER_STDIO_H

#include <stdio.h>

int srvSerialPutChar(char ch, FILE *file);

int srvSerialGetChar(FILE *file);

void srvSerialSetup();

#endif
