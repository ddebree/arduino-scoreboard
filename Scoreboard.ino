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
Bounce timeResetBounce = Bounce(); 

uint8_t period = 1;

Chrono gameChrono(Chrono::MILLIS); 

uint8_t currentDigit = 0;

void setup() {
  //setupKeys();
  setupDigits();
  
  scoreLeft.attach(4, 5);
  scoreRight.attach(6, 7);
}

void loop() {
  //updateKeys();
  updateDigits();

  if (gameChrono.hasPassed(120000)) {
    gameChrono.restart();
  }
}

void updateKeys() {
  //Update the debouncing:
  timeStartBounce.update();
  timeResetBounce.update();

  //Do something about time here...

  scoreLeft.update();
  scoreRight.update();
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
      value = scoreLeft.getBigDigit(); 
      break;
    case SCORE_RIGHT_SMALL: 
      value = scoreLeft.getSmallDigit(); 
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
  return gameChrono.elapsed() / 300; //Should be 1000, but for testing make it faster 
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
  /*
  leftScoreUpBounce.attach();
  leftScoreDownBounce.attach();
  rightScoreUpBounce.attach();
  rightScoreDownBounce.attach();
  timeStartBounce.attach();
  timeResetBounce.attach();


  
  // Setup the first button with an internal pull-up :
  pinMode(BUTTON_PIN_1,INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer1.attach(BUTTON_PIN_1);
  debouncer1.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms
  
   // Setup the second button with an internal pull-up :
  pinMode(BUTTON_PIN_2,INPUT_PULLUP);
  // After setting up the button, setup the Bounce instance :
  debouncer2.attach(BUTTON_PIN_2);
  debouncer2.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms
  */
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
