#include "downtimer.h"

#include <EEPROM.h>

#define GAME_LENGTH_ADDRESS 0
#define SHOT_CLOCK_TIME 60000L

#include "defines.h"

void DownTimer::init() {
  _gameLengthDivide5s = EEPROM.read(GAME_LENGTH_ADDRESS);
  //test: _gameLengthDivide5s = 4;
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

unsigned long DownTimer::getGameTimeToShow() {
  unsigned long gameTime = elapsed();
  if (gameTime < _gameLength) {
    //This makes it show a little more time to make it finsh exactly on zero
    return _gameLength + 999L - gameTime;
  } else {
    return 0;
  }
}

unsigned long DownTimer::getShotTimeToShow() {
  unsigned long gameTimeToShow = getGameTimeToShow();
  unsigned long shotClockTime = shotClockElapsed();
  if (shotClockTime < SHOT_CLOCK_TIME) {
    //Add 999 to make the end right:
    unsigned long shotClockTimeToShow = (SHOT_CLOCK_TIME + 999L) - shotClockTime;
    if (shotClockTimeToShow > gameTimeToShow) {
      return gameTimeToShow;
    } else {
      return shotClockTimeToShow;
    }
  } else {
    return 0;
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

bool DownTimer::hasJustCompleted() {
  //buzzerLong();
  if (isRunning() && isAfterPeriod()) {
    stop();
    return true;
  }
  return false;
}

bool DownTimer::hasShotClockJustCompleted() {
  if (isRunning()
    && ( ! _shotClockExpired)
    && shotClockElapsed() > SHOT_CLOCK_TIME) {
    _shotClockExpired = true;
    return true;
  } else {
    return false;
  }
}

void DownTimer::reset() {
  //Two steps to avoid possible casting issue:
  _gameLength = _gameLengthDivide5s;
  _gameLength *= 5000L;
  _startTime = _offset = 0;
  _isRunning = _shotClockExpired = false;
}

void DownTimer::incGameTime(bool persistTimeChange) {
  if (! isRunning()) {
    if (persistTimeChange) {
      if (_gameLengthDivide5s < 240) {
        _gameLength += 5000L;
        _gameLengthDivide5s++;
        EEPROM.write(GAME_LENGTH_ADDRESS, _gameLengthDivide5s);
      }
    } else {
      _gameLength += 5000L;
    }
  }
}

void DownTimer::decGameTime(boolean persistTimeChange) {
  if (! isRunning()) {
    if (persistTimeChange) {
      if (_gameLengthDivide5s > 12) {
        _gameLength = _gameLength - 5000L;
        _gameLengthDivide5s--;
        EEPROM.write(GAME_LENGTH_ADDRESS, _gameLengthDivide5s);
      }
    } else {
      if (_gameLength > 5000L) {
        _gameLength = _gameLength - 5000L;
      }
    }
    
    
  }
}

void DownTimer::resetShotClock() {
  _shotClockStartElapsed = elapsed();
  _shotClockExpired = false;
}
