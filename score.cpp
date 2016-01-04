#include "score.h"

void Score::attach(uint8_t bigAddress, uint8_t smallAddress) {
  _bigSevenSeg.attach(bigAddress);
  _smallSevenSeg.attach(smallAddress);
}

void Score::updateDigits(uint8_t pwmSize, uint8_t currentAddress) {
  _bigSevenSeg.setValue(getBigDigit());
  _smallSevenSeg.setValue(getSmallDigit());

  _bigSevenSeg.updateDigit(pwmSize, currentAddress);
  _smallSevenSeg.updateDigit(pwmSize, currentAddress);
}

void Score::inc() {
  if (_score < 50) {
    _score++;
  }
}

void Score::dec() {
  if (_score > 0) {
    _score--;
  }
}

void Score::reset() {
  _score = 0;
}

uint8_t Score::getSmallDigit() {
  return _score % 10;
}

uint8_t Score::getBigDigit() {
  return _score / 10;
}

