#include <Wire.h>
#include <Bounce2.h>
#include <Adafruit_MCP23008.h>
#include <Chrono.h>
#include <LightChrono.h>

#include "defines.h"

Adafruit_MCP23008 sevenSegs[8];

Bounce leftScoreUpBounce = Bounce(); 
Bounce leftScoreDownBounce = Bounce(); 
Bounce rightScoreUpBounce = Bounce(); 
Bounce rightScoreDownBounce = Bounce(); 
Bounce timeStartBounce = Bounce(); 
Bounce timeResetBounce = Bounce(); 

byte scoreLeft = 0;
byte scoreRight = 0;
byte period = 1;

Chrono gameChrono(Chrono::MILLIS); 

byte currentDigit = 0;

void setup() {
  //setupKeys();
  setupDigits();
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
  leftScoreUpBounce.update();
  leftScoreDownBounce.update();
  rightScoreUpBounce.update();
  rightScoreDownBounce.update();
  timeStartBounce.update();
  timeResetBounce.update();

  if (leftScoreUpBounce.fell() && scoreLeft < MAX_SCORE) {
    scoreLeft++;
  }
  if (leftScoreDownBounce.fell() && scoreLeft > 0) {
    scoreLeft--;
  }
  if (rightScoreUpBounce.fell() && scoreRight < MAX_SCORE) {
    scoreRight++;
  }
  if (rightScoreDownBounce.fell() && scoreRight > 0) {
    scoreRight--;
  }
}

void updateDigits() {
  sevenSegs[currentDigit].writeGPIO(0);
  
  currentDigit++;
  if (currentDigit >= 8) {
    currentDigit = 0;
  }

  byte value;

  switch (currentDigit) {
    case SCORE_LEFT_BIG: 
      value = getBigDigit(scoreLeft); 
      break;
    case SCORE_LEFT_SMALL: 
      value = getSmallDigit(scoreLeft); 
      break;
    case SCORE_RIGHT_BIG: 
      value = getBigDigit(scoreRight); 
      break;
    case SCORE_RIGHT_SMALL: 
      value = getSmallDigit(scoreRight); 
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

byte getSmallDigit(byte input) {
  while (input > 10) {
    input = input - 10;
  }
  return input;
}

byte getBigDigit(byte input) {
  input = input / 10;
  return getSmallDigit(input);
}

byte getSmallSecond(unsigned long elapsedTime) {
  return byte(elapsedTime % 10);
}

byte getBigSecond(unsigned long elapsedTime) {
  return byte((elapsedTime / 10) % 6);
}

byte getSmallMinute(unsigned long elapsedTime) {
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
