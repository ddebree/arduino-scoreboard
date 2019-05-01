#include "buzzer.h"
#include "Arduino.h"

void Buzzer::init() {
  _buzzerOn = false;
  _buzzerOffTime1 = 0;

  _buzzerOnTime2 = 0;
  _buzzerOffTime2 = 0;
}

bool Buzzer::isBuzzerOn() {
  if (_buzzerOn) {
    if (millis() > _buzzerOffTime1) {
      _buzzerOn = false;
    }
  } else if (_buzzerOnTime2 != 0
          && (millis() > _buzzerOnTime2)) {
    _buzzerOn = true;
    _buzzerOffTime1 = _buzzerOffTime2;
    _buzzerOnTime2 = 0;
    _buzzerOffTime2 = 0;
  }
  return _buzzerOn;
}

void Buzzer::buzzerShort() {
  _buzzerOn = true;
  _buzzerOffTime1 = millis() + 500;
  _buzzerOnTime2 = 0;
  _buzzerOffTime2 = 0;
}

void Buzzer::buzzerShortShort() {
  unsigned long currentTime = millis();
  _buzzerOn = true;
  _buzzerOffTime1 = currentTime + 500L;
  _buzzerOnTime2 = currentTime + 1000L;
  _buzzerOffTime2 = currentTime + 1500L;
}

void Buzzer::buzzerLong() {
  _buzzerOn = true;
  _buzzerOffTime1 = millis() + 2000L;
  _buzzerOnTime2 = 0;
  _buzzerOffTime2 = 0;
}
