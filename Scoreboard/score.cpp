#include "score.h"

#define MAX_SCORE 50
#include "defines.h"

void Score::attach(bool useSmallDigitForSmallScore) {
  _useSmallDigitForSmallScore = useSmallDigitForSmallScore;
}

uint8_t Score::getLeftDigit() {
  return _leftDigit;
}

uint8_t Score::getRightDigit() {
  return _rightDigit;
}

void Score::inc() {
  if (_score < MAX_SCORE) {
    _score++;
    updateDigits();
  }
}

void Score::dec() {
  if (_score > 0) {
    _score--;
    updateDigits();
  }
}

void Score::reset() {
  _score = 0;
  updateDigits();
}

void Score::updateDigits() {
  if (_score >= 10) {
    //Find the small and big score values:
    uint8_t smallScore = _score;
    uint8_t bigScore = 0;
    while (smallScore >= 10) {
      smallScore = smallScore - 10;
      bigScore++;
    }
    _leftDigit = bigScore;
    _rightDigit = smallScore;
  } else {
    if (_useSmallDigitForSmallScore) {
      _leftDigit = BLANK_DIGIT;
      _rightDigit = _score;
    } else {
      _leftDigit = _score;
      _rightDigit = BLANK_DIGIT;
    }
  }
}
