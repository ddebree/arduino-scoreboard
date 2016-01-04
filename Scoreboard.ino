#include <Wire.h>
#include <Bounce2.h>
#include <Adafruit_MCP23008.h>
#include <Chrono.h>
#include <LightChrono.h>

#include "defines.h"
#include "sevenseg.h"
#include "score.h"

SevenSeg sevenSegDigits[8];

Score scoreLeft;
Score scoreRight;

Bounce scoreLeftUpBounce = Bounce();
Bounce scoreLeftDownBounce = Bounce();
Bounce scoreRightUpBounce = Bounce();
Bounce scoreRightDownBounce = Bounce();
Bounce timeStartBounce = Bounce(); 
Bounce resetBounce = Bounce(); 

uint8_t period = 1;

Chrono gameChrono(Chrono::MILLIS); 

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

  if (gameChrono.hasPassed(120000)) {
    gameChrono.restart();
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
    if (gameChrono.isRunning()) {
      gameChrono.stop();
    } else {
      gameChrono.resume();
    }
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

  uint8_t value;

  switch (currentDigit) {
    case SCORE_LEFT_BIG: 
      value = scoreLeft.getBigDigit(); 
      break;
    case SCORE_LEFT_SMALL: 
      value = scoreLeft.getSmallDigit(); 
      break;
    case SCORE_RIGHT_BIG: 
      value = scoreRight.getBigDigit(); 
      break;
    case SCORE_RIGHT_SMALL: 
      value = scoreRight.getSmallDigit(); 
      break;
    case PERIOD: 
      value = period; //This should always be < 9
      break;
    case TIME_SECOND_BIG: 
      value = getBigSecond(getGameTime());
      break;
    case TIME_SECOND_SMALL: 
      value = getSmallSecond(getGameTime());
      break;
    case TIME_MINUTE_SMALL: 
      value = getSmallMinute(getGameTime());
      break;
    default:
      value = 99;
  }
  sevenSegDigits[currentDigit].setValue(value);
  
  for (uint8_t i = 0; i < 8; i++) {
    sevenSegDigits[i].updateDigit(i);
  }
}

unsigned long getGameTime() {
  long scale = 300;
  long timeRemaining = /*600000L - */gameChrono.elapsed();
  
  return timeRemaining / scale; //Should be 1000, but for testing make it faster 
}

uint8_t getSmallSecond(unsigned long elapsedTime) {
  return byte(elapsedTime % 10);
}

uint8_t getBigSecond(unsigned long elapsedTime) {
  return byte((elapsedTime / 10) % 6);
}

uint8_t getSmallMinute(unsigned long elapsedTime) {
  return byte(elapsedTime / 60);
}

void setupDigits() {
  for (uint8_t i = 0; i < 8; i++) {
    sevenSegDigits[i].attach(i);
  }
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
