#ifndef SCORE_H_
#define SCORE_H_

#include <Bounce2.h>
#include <inttypes.h>

class Score {
  public:
    //Constructor:
    Score(int pinUp, int pinDown);

    void updateKeys();

    uint8_t getSmallDigit();
    uint8_t getBigDigit();

  protected:
    uint8_t _score;

    Bounce _upBounce = Bounce();
    Bounce _downBounce = Bounce();

};

#endif
