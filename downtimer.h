#ifndef D_TIMER_H_
#define D_TIMER_H_

#include "Arduino.h"
#include <Chrono.h>

#include "sevenseg.h"

class DownTimer {
  public:
    void attach();
    void updateDigits(uint8_t pwmSize, uint8_t currentAddress);

    void startStop();
    void restart();
    bool isRunning();
    bool isJustPastEndTime();

  protected:
    unsigned long getGameTime();

    unsigned long _gameLength = 10L * 60L * 1000L;

    uint8_t _bigMinute;
    SevenSeg _smallMinute;
    SevenSeg _bigSecond;
    SevenSeg _smallSecond;
    
    Chrono _chrono = Chrono(Chrono::MILLIS);
};

#endif
