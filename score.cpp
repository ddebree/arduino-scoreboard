#include "score.h"

#define MAX_SCORE 50

void Score::attach(uint8_t bigAddress, uint8_t smallAddress) {
  _bigSevenSeg.attach(bigAddress);
  _smallSevenSeg.attach(smallAddress);
}

void Score::updateDigits(uint8_t pwmSize, uint8_t currentAddress) {
  //Find the small and big score values:
  uint8_t smallScore = _score;
  uint8_t bigScore = 0;
  while (smallScore >= 10) {
    smallScore = smallScore - 10;
    bigScore++;
  }
  
  _bigSevenSeg.setValue(bigScore);
  _smallSevenSeg.setValue(smallScore);

  _bigSevenSeg.updateDigit(pwmSize, currentAddress);
  _smallSevenSeg.updateDigit(pwmSize, currentAddress);
}

void Score::inc() {
  if (_score < MAX_SCORE) {
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

