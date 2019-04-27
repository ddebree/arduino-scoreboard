#include "sevenseg.h"

#include "defines.h"

void SevenSeg::attach(uint8_t address) {
  _address = address;
  _digit.begin(address);
  for (uint8_t i = 0; i < 8; i++) {
    _digit.pinMode(i, OUTPUT);
  }
}

void SevenSeg::updateDigit(uint8_t pwmSize, uint8_t currentAddress) {
  if (_visible) {
    //Figure out when to turn off:
    if (currentAddress != _address) {
      //hideDigit();
    }
  } else if (currentAddress == _address) {
    showDigit();
  }
}

void SevenSeg::setValue(uint8_t value) {
  _value = value;
  if (_visible) {
    showDigit();
  }
}

uint8_t SevenSeg::getValue() {
  return _value;
}

void SevenSeg::showDigit() {
  _visible = true;
  _digit.writeGPIO(decodeDigit(_value));
}

void SevenSeg::hideDigit() {
  _visible = false;
  _digit.writeGPIO(0);
}

uint8_t SevenSeg::decodeDigit(uint8_t input) {
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