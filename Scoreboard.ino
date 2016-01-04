#include <Wire.h>
#include <Bounce2.h>
#include <Adafruit_MCP23008.h>
#include <Chrono.h>
#include <LightChrono.h>

#include "defines.h"
#include "score.h"

Adafruit_MCP23008 sevenSegs[8];

Score scoreLeft;
Score scoreRight;

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

  scoreLeft._upBounce.update();
  scoreLeft._downBounce.update();
  scoreRight._upBounce.update();
  scoreRight._downBounce.update();

  if (scoreLeft._upBounce.fell()) {
    scoreLeft.inc();
  }
  if (scoreLeft._downBounce.fell()) {
    scoreLeft.dec();
  }
  if (scoreRight._upBounce.fell()) {
    scoreRight.inc();
  }
  if (scoreRight._downBounce.fell()) {
    scoreRight.dec();
  }
}

void updateDigits() {
  sevenSegs[currentDigit].writeGPIO(0);
  
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

  sevenSegs[currentDigit].writeGPIO(decodeDigit(value));
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
    sevenSegs[i].begin(i);
    for (uint8_t j = 0; j < 8; j++) {
      sevenSegs[i].pinMode(j, OUTPUT);
    }
  }
}

void setupKeys() {
  pinMode(BUTTON_PIN_LEFT_UP, INPUT_PULLUP);
  pinMode(BUTTON_PIN_LEFT_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_PIN_RIGHT_UP, INPUT_PULLUP);
  pinMode(BUTTON_PIN_RIGHT_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_PIN_START, INPUT_PULLUP);
  pinMode(BUTTON_PIN_RESET, INPUT_PULLUP);

  scoreLeft._upBounce.attach(BUTTON_PIN_LEFT_UP);
  scoreLeft._downBounce.attach(BUTTON_PIN_LEFT_DOWN);
  scoreRight._upBounce.attach(BUTTON_PIN_RIGHT_UP);
  scoreRight._downBounce.attach(BUTTON_PIN_RIGHT_DOWN);
  timeStartBounce.attach(BUTTON_PIN_START);
  resetBounce.attach(BUTTON_PIN_RESET);

  scoreLeft._upBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms
  scoreLeft._downBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms 
  scoreRight._upBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms
  scoreRight._downBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms 
  timeStartBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms
  resetBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms 
}

uint8_t decodeDigit(uint8_t input) {
  switch (input) {
    case 0: return DIGIT_0;
    case 1: return DIGIT_1;
    case 2: return DIGIT_2;
    case 3: return DIGIT_3;
    case 4: return DIGIT_4;
    case 5: return DIGIT_5;
    case 6: return DIGIT_6;
    case 7: return DIGIT_7;
    case 8: return DIGIT_8;
    case 9: return DIGIT_9;
    default: return 0;
  }    
}
