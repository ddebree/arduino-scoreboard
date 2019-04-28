#ifndef SEVENSEG_H_
#define SEVENSEG_H_

#include "Arduino.h"
#include <Adafruit_MCP23008.h>

class SevenSeg {
  public:
    void attach(uint8_t address);
    void setValue(uint8_t value);
    uint8_t getValue();

  private:
    uint8_t decodeDigit(uint8_t input);

    uint8_t _address;
    uint8_t _value;
    Adafruit_MCP23008 _digit;
};

#endif
