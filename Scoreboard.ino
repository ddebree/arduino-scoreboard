#include <Wire.h>
#include <Bounce2.h>
#include <Adafruit_MCP23008.h>
#include <Chrono.h>

#include "defines.h"

#include "downtimer.h"
#include "sevenseg.h"
#include "score.h"
#include "buzzer.h"

DownTimer countDownTimer;
Score scoreLeft;
Score scoreRight;
SevenSeg periodSevenSeg;
Buzzer buzzer = Buzzer();

Bounce scoreLeftUpBounce = Bounce();
Bounce scoreLeftDownBounce = Bounce();
Bounce scoreRightUpBounce = Bounce();
Bounce scoreRightDownBounce = Bounce();
Bounce timeStartBounce = Bounce(); 
Bounce resetBounce = Bounce(); 

uint8_t currentDigit = 0;
uint8_t pwmWidth = 1;

uint8_t loopcount = 0;
bool debug = false;

void setup() {
  setupKeys();
  setupDigits();
  buzzer.attach();
  periodSevenSeg.setValue(1);
}

void loop() {
  updateKeys();
  updateDigits();
  buzzer.update();
  if (countDownTimer.isJustPastEndTime()) {
    buzzer.buzzerOn(true, 250);
  }
  
  if (debug) {
    loopcount++;
    if (loopcount == 0) {
      Serial.print("Loop time ");
      Serial.println(millis());
    }
  }
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
  if (resetBounce.fell() && ! countDownTimer.isRunning()) {
    countDownTimer.restart();
    if (periodSevenSeg.getValue() <= 1) {
      periodSevenSeg.setValue(2);
    } else {
      periodSevenSeg.setValue(1);
    }
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

  scoreLeft.updateDigits(pwmWidth, currentDigit);
  scoreRight.updateDigits(pwmWidth, currentDigit);
  countDownTimer.updateDigits(pwmWidth, currentDigit);
  periodSevenSeg.updateDigit(pwmWidth, currentDigit);
}

void setupDigits() {
  scoreLeft.attach(SCORE_LEFT_BIG, SCORE_LEFT_SMALL, false);
  scoreRight.attach(SCORE_RIGHT_BIG, SCORE_RIGHT_SMALL, true);
  periodSevenSeg.attach(PERIOD);
  countDownTimer.attach();
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
