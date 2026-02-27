#include "serial.h"
#include <Arduino.h>

int srvSerialPutChar(char ch, FILE *file) { return Serial.write(ch); }

int srvSerialGetChar(FILE *file) {
  while (!Serial.available())
    ;

  return Serial.read();
}

void srvSerialSetup() {
  Serial.begin(9600);
  FILE *srvSerialStream = fdevopen(srvSerialPutChar, srvSerialGetChar);
  stdin = stdout = srvSerialStream;
}
