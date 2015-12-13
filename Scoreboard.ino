#include <Wire.h>
#include "Adafruit_MCP23008.h"

#include "defines.h"

Adafruit_MCP23008 sevenSegs[8];

void setup() {
  for (uint8_t i = 0; i < 8; i++) {
    sevenSegs[i].begin(i);
    for (uint8_t j = 0; j < 8; j++) {
      sevenSegs[i].pinMode(j, OUTPUT);
    }
  }
}

void loop() {
  for (uint8_t i = 0; i < 8; i++) {
    for (uint8_t j = 0; j < 10; j++) {
      sevenSegs[i].writeGPIO(decodeDigit(j));
      delay(300);
      sevenSegs[i].writeGPIO(0);
    }
  }
}

uint8_t decodeDigit(uint8_t input) {
  switch (input) {
    case 0: return DIGIT_0;
    case 1: return DIGIT_1;
    case 2: return DIGIT_2;
    case 3: return DIGIT_3;
    case 4: return DIGIT_4;
    case 5: return DIGIT_5;
    case 6: return DIGIT_6;
    case 7: return DIGIT_7;
    case 8: return DIGIT_8;
    case 9: return DIGIT_9;
    default: return 0;
  }    
}
