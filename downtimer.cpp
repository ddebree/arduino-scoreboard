#include "downtimer.h"

#include "defines.h"

void DownTimer::attach(uint8_t smallMinuteAddress, uint8_t bigSecondAddress, uint8_t smallSecondAddress) {
  _smallMinute.attach(smallMinuteAddress);
  _bigSecond.attach(bigSecondAddress);
  _smallSecond.attach(smallSecondAddress);
}

void DownTimer::updateDigits(uint8_t pwmSize, uint8_t currentAddress) {
  unsigned long elapsedTime = getGameTime();
  
  _smallMinute.setValue(getSmallMinute(elapsedTime));
  _bigSecond.setValue(getBigSecond(elapsedTime));
  _smallSecond.setValue(getSmallSecond(elapsedTime));
  
  _smallMinute.updateDigit(pwmSize, currentAddress);
  _bigSecond.updateDigit(pwmSize, currentAddress);
  _smallSecond.updateDigit(pwmSize, currentAddress);
}

unsigned long DownTimer::getGameTime() {
  long scale = 300;
  long timeRemaining = /*600000L - */_chrono.elapsed();
  
  return timeRemaining / scale; //Should be 1000, but for testing make it faster 
}

uint8_t DownTimer::getSmallSecond(unsigned long elapsedTime) {
  return byte(elapsedTime % 10);
}

uint8_t DownTimer::getBigSecond(unsigned long elapsedTime) {
  return byte((elapsedTime / 10) % 6);
}

uint8_t DownTimer::getSmallMinute(unsigned long elapsedTime) {
  return byte(elapsedTime / 60);
}
