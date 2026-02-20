#include "keypad.h"
#include "Key.h"
#include <Keypad.h>
#include <stdio.h>

#define SERVICE_KEYPAD_REPEAT_DELAY 100

const byte ROWS = 4;  // four rows
const byte COLS = 4;  // four columns
// define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = { { '1', '2', '3', 'A' },
                              { '4', '5', '6', 'B' },
                              { '7', '8', '9', 'C' },
                              { '*', '0', '#', 'D' } };
byte rowPins[ROWS] = { 2, 3, 4, 5 };  // connect to the row pinouts of the keypad
byte colPins[COLS] = { 6, 7, 8, 9 };  // connect to the column pinouts of the
                                      // keypad

Keypad customKeypad =
  Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

int srvKeypadGetKey(FILE *stream) {
  char customKey;

  do {
    customKey = customKeypad.getKey();
  } while (customKey == NO_KEY);

  delay(SERVICE_KEYPAD_REPEAT_DELAY);

  return (int)customKey;
}

void srvKeypadSetup() {
  FILE *myStream = fdevopen(NULL, srvKeypadGetKey);

  stdin = myStream;
}
