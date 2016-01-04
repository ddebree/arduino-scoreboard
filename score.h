#ifndef SCORE_H_
#define SCORE_H_

#include <Bounce2.h>

#include "sevenseg.h"

class Score {
  public:
    void attach(uint8_t bigAddress, uint8_t smallAddress);
    void updateDigits(uint8_t pwmSize, uint8_t currentAddress);
  
    void inc();
    void dec();
    void reset();

    uint8_t getSmallDigit();
    uint8_t getBigDigit();

  protected:
    uint8_t _score;
    SevenSeg _bigSevenSeg;
    SevenSeg _smallSevenSeg;
};

#endif
