#ifndef D_TIMER_H_
#define D_TIMER_H_

#include "Arduino.h"
#include <Chrono.h>

#include "sevenseg.h"

class DownTimer : public Chrono {
  public:
    void init();
    void setFastTime();

    unsigned long elapsed() const;

    void startStop();
    void reset();

    bool hasJustCompleted();
    bool hasShotClockJustCompleted();

    bool isAfterPeriod();

    unsigned long shotClockElapsed();

    unsigned long getGameTimeToShow();
    unsigned long getShotTimeToShow();

    void incGameTime(bool persistTimeChange);
    void decGameTime(bool persistTimeChange);
    void resetShotClock();

  protected:
    byte _gameLengthDivide5s;
    unsigned long _gameLength;
    bool _fastTime = false;

    unsigned long _shotClockStartElapsed;
    bool _shotClockExpired;

    uint8_t _bigMinute;
};

#endif
