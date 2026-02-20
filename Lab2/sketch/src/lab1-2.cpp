#include "lab1-2.h"
#include "drivers/led/led.h"
#include "services/keypad/keypad.h"
#include "services/lcd/lcd.h"

#include <stdio.h>
#include <string.h>

void lab12Setup() {
  srvLcdSetup();
  srvKeypadSetup();
  setupGreenLed();
  setupRedLed();
  printf("Enter the access code");
}

char code[4] = {'1', '4', '2', '3'};
char codeDumb[4] = {'1', '1', '1', '1'};
char input[4];
void lab12Loop() {
  if (scanf("%c%c%c%c", &input[0], &input[1], &input[2], &input[3]) == 4) {

    if (memcmp(input, code, 4) == 0) {
      srvLcdClearChar();
      printf("Access Granted\n");
      ledGreenOn();
      ledRedOff();
      delay(3000);
      ledGreenOff();
      srvLcdClearChar();

    } else if (memcmp(input, codeDumb, 4) == 0) {
      srvLcdClearChar();
      printf("Dumb code!!\n");
      ledSOS();
      delay(3000);
      srvLcdClearChar();
    } else {
      srvLcdClearChar();
      printf("Access Denied\n");
      ledGreenOff();
      ledRedOn();
      delay(3000);
      ledRedOff();
      srvLcdClearChar();
    }
  }
}
