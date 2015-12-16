#include "score.h"

Score::Score(int pinUp, int pinDown) {
  _upBounce.attach(pinUp);
  _downBounce.attach(pinDown);
}

void Score::updateKeys() {
  //Update the debouncing:
  _upBounce.update();
  _downBounce.update();

  if (_upBounce.fell() && _score < 50) {
    _score++;
  }
  if (_downBounce.fell() && _score > 0) {
    _score--;
  }
}

uint8_t Score::getSmallDigit() {
  return _score % 10;
}

uint8_t Score::getBigDigit() {
  return _score / 10;
}

