#ifndef SCORE_H_
#define SCORE_H_

#include <Bounce2.h>

class Score {
  public:
    void attach(int pinUp, int pinDown);

    void update();

    void reset();

    uint8_t getSmallDigit();
    uint8_t getBigDigit();

  protected:
    uint8_t _score;

    Bounce _upBounce = Bounce();
    Bounce _downBounce = Bounce();

};

#endif
