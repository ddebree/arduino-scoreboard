#include "score.h"

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

