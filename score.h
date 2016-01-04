#ifndef SCORE_H_
#define SCORE_H_

#include <Bounce2.h>

class Score {
  public:
    void inc();
    void dec();
    void reset();

    uint8_t getSmallDigit();
    uint8_t getBigDigit();

    Bounce _upBounce = Bounce();
    Bounce _downBounce = Bounce();

  protected:
    uint8_t _score;
};

#endif
