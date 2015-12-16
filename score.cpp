#include "score.h"

#include "defines.h"
#include "Arduino.h"

void Score::attach(int pinUp, int pinDown) {
  pinMode(pinUp, INPUT_PULLUP);
  pinMode(pinDown, INPUT_PULLUP);
  
  _upBounce.attach(pinUp);
  _downBounce.attach(pinDown);
  
  _upBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms
  _downBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms 
}

void Score::update() {
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

void Score::reset() {
  _score = 0;
}

uint8_t Score::getSmallDigit() {
  return _score % 10;
}

uint8_t Score::getBigDigit() {
  return _score / 10;
}

