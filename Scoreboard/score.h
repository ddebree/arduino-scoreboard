#ifndef SCORE_H_
#define SCORE_H_

#include <Bounce2.h>

#include "sevenseg.h"

class Score {
  public:
    void init(bool useSmallDigitForSmallScore);

    uint8_t getLeftDigit();
    uint8_t getRightDigit();

    void inc();
    void dec();
    void reset();

  protected:
    uint8_t _score;
    bool _useSmallDigitForSmallScore;

    uint8_t _rightDigit;
    uint8_t _leftDigit;
  private:
    void updateDigits();
};

#endif
