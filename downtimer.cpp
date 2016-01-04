#include "downtimer.h"

#include "defines.h"

void DownTimer::attach(uint8_t smallMinuteAddress, uint8_t bigSecondAddress, uint8_t smallSecondAddress) {
  _smallMinute.attach(smallMinuteAddress);
  _bigSecond.attach(bigSecondAddress);
  _smallSecond.attach(smallSecondAddress);
}

void DownTimer::updateDigits(uint8_t pwmSize, uint8_t currentAddress) {
  unsigned long elapsedTime = getGameTime();

  uint8_t minute10 = 0;
  uint8_t minute = 0;
  uint8_t second10 = 0;
  uint8_t second = 0;

  while (elapsedTime >= 600000L) {
    minute10++;
    elapsedTime = elapsedTime - 600000L;
  }
  while (elapsedTime >= 60000L) {
    minute++;
    elapsedTime = elapsedTime - 60000L;
  }
  while (elapsedTime >= 10000L) {
    second10++;
    elapsedTime = elapsedTime - 10000L;
  }
  while (elapsedTime >= 1000L) {
    second++;
    elapsedTime = elapsedTime - 1000L;
  }

  _bigMinute = minute10;
  _smallMinute.setValue(minute);
  _bigSecond.setValue(second10);
  _smallSecond.setValue(second);
  
  _smallMinute.updateDigit(pwmSize, currentAddress);
  _bigSecond.updateDigit(pwmSize, currentAddress);
  _smallSecond.updateDigit(pwmSize, currentAddress);

  if (_chrono.hasPassed(120000)) {
    _chrono.restart();
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
  long scale = 300;
  long timeRemaining = /*600000L - */_chrono.elapsed();
  
  return timeRemaining / scale; //Should be 1000, but for testing make it faster 
}
