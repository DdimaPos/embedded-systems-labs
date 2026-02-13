#include "lab1-1.h"
#include "drivers/led/led.h"
#include "services/serial/serial.h"

#include <stdio.h>
#include <string.h>

void lab11Setup() {
  srvSerialSetup();
  setupLed();

  printf("\n[DEBUG] System Ready. type led on/led off to control the led\n");
}

char command[20];
void lab11Loop() {
  fgets(command, sizeof(command), stdin);
  command[strcspn(command, "\n")] = '\0';

  if (strcmp(command, "led on") == 0) {
    ledOn();
    printf("[INFO] Led was turned on\n");
  } else if (strcmp(command, "led off") == 0) {
    ledOff();
    printf("[INFO] Led was turned off\n");
  } else if (strcmp(command, "led sos") == 0) {
    ledSOS();
    printf("[SOS] SOS SOS\n");
  } else {
    printf("[WARNING] Unknown command\n");
  }
}
