#include "buzzer.h"

#include "defines.h"

void Buzzer::attach() {
  pinMode(PIN_BUZZER_HIGH, OUTPUT);
  pinMode(PIN_BUZZER_LOW, OUTPUT);
  digitalWrite(PIN_BUZZER_HIGH, LOW);
  digitalWrite(PIN_BUZZER_LOW, LOW);
}

void Buzzer::update() {
  if (_buzzerOn) {
    if (millis() > _offTime) {
      _buzzerOn = false;
      digitalWrite(PIN_BUZZER_HIGH, LOW);
      digitalWrite(PIN_BUZZER_LOW, LOW);
    }
  }
}

void Buzzer::buzzerOn(bool highPitch, unsigned long onTime) {
  _buzzerOn = true;
  _offTime = millis() + onTime;
  if (highPitch) {
    digitalWrite(PIN_BUZZER_HIGH, HIGH);
  } else {
    digitalWrite(PIN_BUZZER_LOW, HIGH);
  }
}

