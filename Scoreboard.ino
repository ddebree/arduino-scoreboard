#include <Wire.h>
#include <Bounce2.h>
#include <Adafruit_MCP23008.h>
#include <Chrono.h>

#include "defines.h"

#include "downtimer.h"
#include "sevenseg.h"
#include "score.h"

SevenSeg periodSevenSeg;

DownTimer countDownTimer;

Score scoreLeft;
Score scoreRight;

Bounce scoreLeftUpBounce = Bounce();
Bounce scoreLeftDownBounce = Bounce();
Bounce scoreRightUpBounce = Bounce();
Bounce scoreRightDownBounce = Bounce();
Bounce timeStartBounce = Bounce(); 
Bounce resetBounce = Bounce(); 

uint8_t currentDigit = 0;

void setup() {
  setupKeys();
  setupDigits();

  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);

  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
}

void loop() {
  updateKeys();
  updateDigits();
}

void updateKeys() {
  //Update the debouncing:
  scoreLeftUpBounce.update();
  scoreLeftDownBounce.update();
  scoreRightUpBounce.update();
  scoreRightDownBounce.update();
  timeStartBounce.update();
  resetBounce.update();

  if (timeStartBounce.fell()) {
    countDownTimer.startStop();
  }
  if (resetBounce.read()) {
    digitalWrite(8, LOW);
  } else {
    digitalWrite(8, HIGH);
  }
  if (resetBounce.fell()) {
    //TODO
  }

  if (scoreLeftUpBounce.fell()) {
    scoreLeft.inc();
  }
  if (scoreLeftDownBounce.fell()) {
    scoreLeft.dec();
  }
  if (scoreRightUpBounce.fell()) {
    scoreRight.inc();
  }
  if (scoreRightDownBounce.fell()) {
    scoreRight.dec();
  }
}

void updateDigits() {
  currentDigit++;
  if (currentDigit >= 8) {
    currentDigit = 0;
  }

  scoreLeft.updateDigits(1, currentDigit);
  scoreRight.updateDigits(1, currentDigit);
  countDownTimer.updateDigits(1, currentDigit);
  periodSevenSeg.updateDigit(1, currentDigit);
}

void setupDigits() {
  scoreLeft.attach(SCORE_LEFT_BIG, SCORE_LEFT_SMALL);
  scoreRight.attach(SCORE_RIGHT_BIG, SCORE_RIGHT_SMALL);
  periodSevenSeg.attach(PERIOD);
  countDownTimer.attach(TIME_MINUTE_SMALL, TIME_SECOND_BIG, TIME_SECOND_SMALL);
}

void setupKeys() {
  pinMode(BUTTON_PIN_LEFT_UP, INPUT_PULLUP);
  pinMode(BUTTON_PIN_LEFT_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_PIN_RIGHT_UP, INPUT_PULLUP);
  pinMode(BUTTON_PIN_RIGHT_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_PIN_START, INPUT_PULLUP);
  pinMode(BUTTON_PIN_RESET, INPUT_PULLUP);

  scoreLeftUpBounce.attach(BUTTON_PIN_LEFT_UP);
  scoreLeftDownBounce.attach(BUTTON_PIN_LEFT_DOWN);
  scoreRightUpBounce.attach(BUTTON_PIN_RIGHT_UP);
  scoreRightDownBounce.attach(BUTTON_PIN_RIGHT_DOWN);
  timeStartBounce.attach(BUTTON_PIN_START);
  resetBounce.attach(BUTTON_PIN_RESET);

  scoreLeftUpBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms
  scoreLeftDownBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms 
  scoreRightUpBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms
  scoreRightDownBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms 
  timeStartBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms
  resetBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms 
}
