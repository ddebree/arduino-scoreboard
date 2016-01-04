#ifndef SEVENSEG_H_
#define SEVENSEG_H_

#include "Arduino.h"
#include <Adafruit_MCP23008.h>

class SevenSeg {
  public:
    void attach(uint8_t address);
    void setValue(uint8_t value);
    void updateDigit(uint8_t pwmSize, uint8_t currentAddress);
    uint8_t getValue();

  protected:
    uint8_t decodeDigit(uint8_t input);
    
    uint8_t _address;
    uint8_t _value;
    boolean _visible;
    Adafruit_MCP23008 _digit;
};

#endif
