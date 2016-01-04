#ifndef D_TIMER_H_
#define D_TIMER_H_

#include "Arduino.h"
#include <Chrono.h>

#include "sevenseg.h"

class DownTimer {
  public:
    void attach(uint8_t smallMinuteAddress, uint8_t bigSecondAddress, uint8_t smallSecondAddress);
    void updateDigits(uint8_t pwmSize, uint8_t currentAddress);

    void startStop();
  
    unsigned long getGameTime();

    uint8_t _bigMinute;
    SevenSeg _smallMinute;
    SevenSeg _bigSecond;
    SevenSeg _smallSecond;
    
    Chrono _chrono = Chrono(Chrono::MILLIS);
};

#endif
