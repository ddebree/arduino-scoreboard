#include "score.h"

#define MAX_SCORE 50
#include "defines.h"

void Score::attach(uint8_t bigAddress, uint8_t smallAddress, bool useSmallDigitForSmallScore) {
  _bigSevenSeg.attach(bigAddress);
  _smallSevenSeg.attach(smallAddress);
  _useSmallDigitForSmallScore = useSmallDigitForSmallScore;
}

uint8_t Score::getSmallScore() {
  return _score % 10;
}

uint8_t Score::getBigScore() {
  if (_score > 10) {
    return _score / 10;
  } else {
    return 0;
  }
}

void Score::updateDigits(uint8_t currentAddress) {
  if (_score >= 10) {
    //Find the small and big score values:
    uint8_t smallScore = _score;
    uint8_t bigScore = 0;
    while (smallScore >= 10) {
      smallScore = smallScore - 10;
      bigScore++;
    }
    _bigSevenSeg.setValue(bigScore);
    _smallSevenSeg.setValue(smallScore);
  } else {
    if (_useSmallDigitForSmallScore) {
      _bigSevenSeg.setValue(BLANK_DIGIT);
      _smallSevenSeg.setValue(_score);
    } else {
      _bigSevenSeg.setValue(_score);
      _smallSevenSeg.setValue(BLANK_DIGIT);
    }
  }

  _bigSevenSeg.updateDigit(currentAddress);
  _smallSevenSeg.updateDigit(currentAddress);
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
