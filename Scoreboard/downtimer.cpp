#include "downtimer.h"

#include <EEPROM.h>

#define GAME_LENGTH_ADDRESS 0

#include "defines.h"

void DownTimer::init() {
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

unsigned long DownTimer::getGameTimeToShow() {
  unsigned long gameTime = elapsed();
  unsigned long timeToShow = 0;
  if (gameTime < _gameLength) {
    timeToShow = _gameLength - gameTime;
    //This makes it show a little more time to make it finsh exactly on zero
    timeToShow += 999L;
  }
  return timeToShow;
}

unsigned long DownTimer::getShotTimeToShow() {
  //60000L - elapsedShotTime

  return elapsed() - _shotClockStartElapsed;
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
  //buzzerLong();
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
