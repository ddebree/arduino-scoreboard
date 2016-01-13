#include "downtimer.h"

#include "defines.h"

void DownTimer::attach(uint8_t smallMinuteAddress, uint8_t bigSecondAddress, uint8_t smallSecondAddress) {
  _smallMinute.attach(smallMinuteAddress);
  _bigSecond.attach(bigSecondAddress);
  _smallSecond.attach(smallSecondAddress);
}

void DownTimer::updateDigits(uint8_t pwmSize, uint8_t currentAddress) {
  unsigned long gameTime = getGameTime();

  unsigned long timeToShow = 0;
  if (gameTime > _gameLength) {
    timeToShow = 0;
    _chrono.stop();
  } else {
    timeToShow = _gameLength - gameTime;
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
  if (currentAddress == 0 && _bigMinute > 0) {
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

void DownTimer::startStop() {
  if (_chrono.isRunning()) {
      _chrono.stop();
    } else {
      _chrono.resume();
    }
}

unsigned long DownTimer::getGameTime() {
  return 3L * _chrono.elapsed();
  
  //return timeRemaining / scale; //Should be 1000, but for testing make it faster 
}
