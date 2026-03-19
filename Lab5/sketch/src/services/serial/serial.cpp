#include "serial.h"
#include <Arduino.h>

void srvSerialSetup() {
  Serial.begin(115200);
}
