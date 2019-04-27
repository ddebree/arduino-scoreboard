#ifndef D_TIMER_H_
#define D_TIMER_H_

#include "Arduino.h"
#include <Chrono.h>

#include "sevenseg.h"

class DownTimer : public Chrono {
  public:
    void attach();
    void setFastTime();

    unsigned long elapsed() const;

    void startStop();
    void reset();
    bool onPeriodComplete();

    bool isAfterPeriod();

    unsigned long getGameTimeToShow();
    unsigned long getShotTimeToShow();

    void incGameTime();
    void decGameTime();
    void resetShotClock();

  protected:
    byte _gameLengthDivide5s;
    unsigned long _gameLength;
    bool _fastTime = false;

    unsigned long _shotClockStartElapsed;

    uint8_t _bigMinute;
};

#endif
