#include "downtimer.h"

#include <EEPROM.h>

#define GAME_LENGTH_ADDRESS 0

#include "defines.h"

void DownTimer::attach() {
  _smallMinute.attach(TIME_MINUTE_SMALL);
  _bigSecond.attach(TIME_SECOND_BIG);
  _smallSecond.attach(TIME_SECOND_SMALL);

  pinMode(PIN_DOTS, OUTPUT);
  pinMode(PIN_BIG_MINUTE, OUTPUT);

  digitalWrite(PIN_DOTS, LOW);
  digitalWrite(PIN_BIG_MINUTE, LOW);

  _gameLengthDivide5s = EEPROM.read(GAME_LENGTH_ADDRESS);
  //test: _gameLengthDivide5s = 4;
  _gameLength = _gameLengthDivide5s;
  _gameLength *= 5000L;
  reset();
}

unsigned long DownTimer::elapsed() const {
  if (_fastTime) {
    return Chrono::elapsed() << 2;
  } else {
    return Chrono::elapsed();
  }
}

unsigned long DownTimer::shotClockElapsed() {
  return elapsed() - _shotClockStartElapsed;
}

void DownTimer::updateDigits(uint8_t pwmSize, uint8_t currentAddress) {
  unsigned long gameTime = elapsed();

  unsigned long timeToShow = 0;
  if (gameTime < _gameLength) {
    timeToShow = _gameLength - gameTime;
    //This makes it show a little more time to make it finsh exactly on zero
    timeToShow += 999L;
  }
  
  uint8_t minute10 = 0;
  uint8_t minute = 0;
  uint8_t second10 = 0;
  uint8_t second = 0;

  while (timeToShow >= 600000L) {
    minute10++;
    timeToShow = timeToShow - 600000L;
  }
  while (timeToShow >= 60000L) {
    minute++;
    timeToShow = timeToShow - 60000L;
  }
  while (timeToShow >= 10000L) {
    second10++;
    timeToShow = timeToShow - 10000L;
  }
  while (timeToShow >= 1000L) {
    second++;
    timeToShow = timeToShow - 1000L;
  }

  _bigMinute = minute10;
  _smallMinute.setValue(minute);
  _bigSecond.setValue(second10);
  _smallSecond.setValue(second);
  
  _smallMinute.updateDigit(pwmSize, currentAddress);
  _bigSecond.updateDigit(pwmSize, currentAddress);
  _smallSecond.updateDigit(pwmSize, currentAddress);
  if (/*currentAddress == 0 && */_bigMinute > 0) {
    digitalWrite(PIN_BIG_MINUTE, HIGH);
  } else {
    digitalWrite(PIN_BIG_MINUTE, LOW);
  }
  //Show the dots at the same time as the period - It uses less power...
  if (currentAddress == PERIOD) {
    digitalWrite(PIN_DOTS, HIGH);
  } else {
    //digitalWrite(PIN_DOTS, LOW);
  }  
}

void DownTimer::setFastTime() {
  _fastTime = true;
}

void DownTimer::startStop() {
  if (isRunning()) {
      stop();
    } else {
      resume();
    }
}

bool DownTimer::isAfterPeriod() {
  return hasPassed(_gameLength);
}

bool DownTimer::onPeriodComplete() {
  if (isRunning() && isAfterPeriod()) {
    stop();
    return true;
  }
  return false;
}

void DownTimer::reset() {
  _startTime = _offset = 0;
  _isRunning = false;
}

void DownTimer::incGameTime() {
  if (! isRunning()) {
    if (_gameLengthDivide5s < 240) {
      _gameLengthDivide5s++;
      _gameLength += 5000L;
      EEPROM.write(GAME_LENGTH_ADDRESS, _gameLengthDivide5s);
    }
  }
}

void DownTimer::decGameTime() {
  if (! isRunning()) {
    if (_gameLengthDivide5s > 12) {
      _gameLengthDivide5s--;
      _gameLength = _gameLength - 5000L;
      EEPROM.write(GAME_LENGTH_ADDRESS, _gameLengthDivide5s);
    }
  }
}

void DownTimer::resetShotClock() {
  _shotClockStartElapsed = elapsed();
}
