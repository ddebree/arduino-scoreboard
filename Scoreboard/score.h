#ifndef SCORE_H_
#define SCORE_H_

#include <Bounce2.h>

#include "sevenseg.h"

class Score {
  public:
    void attach(uint8_t bigAddress, uint8_t smallAddress, bool useSmallDigitForSmallScore);
    void updateDigits(uint8_t currentAddress);
  
    void inc();
    void dec();
    void reset();

  protected:
    uint8_t _score;
    bool _useSmallDigitForSmallScore;
    SevenSeg _bigSevenSeg;
    SevenSeg _smallSevenSeg;
};

#endif
