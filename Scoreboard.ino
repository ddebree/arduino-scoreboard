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
      delay(200);
      sevenSegs[i].writeGPIO(0);
    }
  }
}

uint8_t decodeDigit(uint8_t input) {
  if (input == 0) {
    return DIGIT_TOP | DIGIT_BOTTOM | DIGIT_LEFT_TOP | DIGIT_LEFT_BOTTOM | DIGIT_RIGHT_TOP | DIGIT_RIGHT_BOTTOM;
  } else if (input == 1) {
    return DIGIT_RIGHT_TOP | DIGIT_RIGHT_BOTTOM;
  } else if (input == 2) {
    return DIGIT_TOP | DIGIT_MIDDLE | DIGIT_BOTTOM | DIGIT_LEFT_BOTTOM | DIGIT_RIGHT_TOP;
  } else if (input == 3) {
    return DIGIT_TOP | DIGIT_MIDDLE | DIGIT_BOTTOM | DIGIT_RIGHT_TOP | DIGIT_RIGHT_BOTTOM;
  } else if (input == 4) {
    return DIGIT_MIDDLE | DIGIT_LEFT_TOP | DIGIT_RIGHT_TOP | DIGIT_RIGHT_BOTTOM;
  } else if (input == 5) {
    return DIGIT_TOP | DIGIT_MIDDLE | DIGIT_BOTTOM | DIGIT_LEFT_TOP | DIGIT_RIGHT_BOTTOM;
  } else if (input == 6) {
    return DIGIT_TOP | DIGIT_MIDDLE | DIGIT_BOTTOM | DIGIT_LEFT_TOP | DIGIT_LEFT_BOTTOM | DIGIT_RIGHT_BOTTOM;
  } else if (input == 7) {
    return DIGIT_TOP | DIGIT_RIGHT_TOP | DIGIT_RIGHT_BOTTOM;
  } else if (input == 8) {
    return DIGIT_TOP | DIGIT_MIDDLE | DIGIT_BOTTOM | DIGIT_LEFT_TOP | DIGIT_LEFT_BOTTOM | DIGIT_RIGHT_TOP | DIGIT_RIGHT_BOTTOM;
  } else if (input == 9) {
    return DIGIT_TOP | DIGIT_MIDDLE | DIGIT_LEFT_TOP | DIGIT_RIGHT_TOP | DIGIT_RIGHT_BOTTOM;
  } else {
    return 0;
  }      
}
