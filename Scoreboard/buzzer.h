#ifndef BUZZER_H_
#define BUZZER_H_

#include "Arduino.h"

class Buzzer {
  public:
    void attach();
    void update();
    void buzzerOn(bool highPitch, unsigned long onTime);

  protected:
    bool _buzzerOn;
    unsigned long _offTime;
};

#endif
