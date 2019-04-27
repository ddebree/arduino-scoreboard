#include <Wire.h>
#include <Bounce2.h>
#include <Adafruit_MCP23008.h>
#include <Chrono.h>
#include <EEPROM.h>
#include <NRFLite.h>

#include "defines.h"

#include "downtimer.h"
#include "sevenseg.h"
#include "score.h"

DownTimer countDownTimer;
Score scoreLeft;
Score scoreRight;
SevenSeg periodSevenSeg;

Bounce scoreLeftUpBounce = Bounce();
Bounce scoreLeftDownBounce = Bounce();
Bounce scoreRightUpBounce = Bounce();
Bounce scoreRightDownBounce = Bounce();
Bounce timeStartBounce = Bounce(); 
Bounce resetBounce = Bounce(); 
Bounce shotTimeBounce = Bounce(); 

uint8_t currentDigit = 0;

struct RadioPacket {
    boolean clockRunning;
    boolean clockVisible;
    unsigned long currentTime;
};

NRFLite radio;
RadioPacket radioData;
unsigned long nextRadioSendTime = 0;

void setup() {
  setupKeys();

  //Setup the digits:
  scoreLeft.attach(SCORE_LEFT_BIG, SCORE_LEFT_SMALL, false);
  scoreRight.attach(SCORE_RIGHT_BIG, SCORE_RIGHT_SMALL, true);
  periodSevenSeg.attach(PERIOD);
  countDownTimer.attach();

  periodSevenSeg.setValue(1);

  //Make the timer run fast for testing...
  delay(KEY_DEBOUNCE_INTERVAL + 1);
  if ( ! resetBounce.read()) {
    countDownTimer.setFastTime();
  }
  Serial.begin(9600);
  if ( ! radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN)) {
    Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.
  }
}

void loop() {
  //Serial.println("Loop Start");
  updateKeys();
  
  currentDigit = (currentDigit + 1) % 8;
  scoreLeft.updateDigits(currentDigit);
  scoreRight.updateDigits(currentDigit);
  countDownTimer.updateDigits(currentDigit);
  periodSevenSeg.updateDigit(currentDigit);

  //We only send the radio data occasionally:
  if (millis() > nextRadioSendTime) {
    Serial.println("Sending radio data");
    unsigned long elapsedShotTime = countDownTimer.getShotTimeToShow();
    if (elapsedShotTime < 60000) {
      radioData.clockRunning = countDownTimer.isRunning();
      radioData.clockVisible = true;
      radioData.currentTime = 60000 - elapsedShotTime;
    } else {
      radioData.clockRunning = false;
      radioData.clockVisible = true;
      radioData.currentTime = 0;
    }
    
    radio.send(RADIO_ID, &radioData, sizeof(radioData));
    nextRadioSendTime = millis() + RADIO_SEND_INTERVAL;
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
  shotTimeBounce.update();

  if (timeStartBounce.rose()) {
    if ( ! resetBounce.read()) {
      countDownTimer.reset();
    } else if (countDownTimer.isAfterPeriod()) {
      countDownTimer.reset();
      alternatePeriod();
    } else {
      countDownTimer.startStop();
      nextRadioSendTime = millis();
    }
  }
  if (resetBounce.rose() && ! countDownTimer.isRunning()) {
    alternatePeriod();
  }

  if (scoreLeftUpBounce.rose()) {
    if (resetBounce.read()) {
      scoreLeft.inc();
    } else {
      countDownTimer.incGameTime();
    }
  }
  if (scoreLeftDownBounce.rose()) {
    if (resetBounce.read()) {
      scoreLeft.dec();
    } else {
      countDownTimer.decGameTime();
    }
  }
  if (scoreRightUpBounce.rose()) {
    scoreRight.inc();
  }
  if (scoreRightDownBounce.rose()) {
    scoreRight.dec();
  }
  if (shotTimeBounce.rose()) {
    //TODO: 
  }
}

void alternatePeriod() {
  if (periodSevenSeg.getValue() <= 1) {
    periodSevenSeg.setValue(2);
  } else {
    periodSevenSeg.setValue(1);
  }
}

void setupKeys() {
  pinMode(BUTTON_PIN_LEFT_UP, INPUT_PULLUP);
  pinMode(BUTTON_PIN_LEFT_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_PIN_RIGHT_UP, INPUT_PULLUP);
  pinMode(BUTTON_PIN_RIGHT_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_PIN_START, INPUT_PULLUP);
  pinMode(BUTTON_PIN_RESET, INPUT_PULLUP);
  pinMode(BUTTON_PIN_SHOT_CLOCK, INPUT_PULLUP);

  scoreLeftUpBounce.attach(BUTTON_PIN_LEFT_UP);
  scoreLeftDownBounce.attach(BUTTON_PIN_LEFT_DOWN);
  scoreRightUpBounce.attach(BUTTON_PIN_RIGHT_UP);
  scoreRightDownBounce.attach(BUTTON_PIN_RIGHT_DOWN);
  timeStartBounce.attach(BUTTON_PIN_START);
  resetBounce.attach(BUTTON_PIN_RESET);
  shotTimeBounce.attach(BUTTON_PIN_SHOT_CLOCK);

  scoreLeftUpBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms
  scoreLeftDownBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms 
  scoreRightUpBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms
  scoreRightDownBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms 
  timeStartBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms
  resetBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms
  shotTimeBounce.interval(KEY_DEBOUNCE_INTERVAL); // interval in ms
}
