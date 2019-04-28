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

SevenSeg scoreLeftLeft;
SevenSeg scoreLeftRight;
SevenSeg scoreRightLeft;
SevenSeg scoreRightRight;

SevenSeg smallMinute;
SevenSeg bigSecond;
SevenSeg smallSecond;

SevenSeg periodSevenSeg;

Bounce scoreLeftUpBounce = Bounce();
Bounce scoreLeftDownBounce = Bounce();
Bounce scoreRightUpBounce = Bounce();
Bounce scoreRightDownBounce = Bounce();
Bounce timeStartBounce = Bounce();
Bounce resetBounce = Bounce();
Bounce shotTimeBounce = Bounce();

uint8_t currentDigit = 0;

bool _buzzerOn;
unsigned long _buzzerOffTime;

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
  scoreLeft.attach(false);
  scoreRight.attach(true);

  scoreLeftLeft.attach(SCORE_LEFT_BIG);
  scoreLeftRight.attach(SCORE_LEFT_SMALL);
  scoreRightLeft.attach(SCORE_RIGHT_BIG);
  scoreRightRight.attach(SCORE_RIGHT_SMALL);

  periodSevenSeg.attach(PERIOD);
  smallMinute.attach(TIME_MINUTE_SMALL);
  bigSecond.attach(TIME_SECOND_BIG);
  smallSecond.attach(TIME_SECOND_SMALL);

  pinMode(PIN_DOTS, OUTPUT);
  pinMode(PIN_BIG_MINUTE, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  digitalWrite(PIN_DOTS, HIGH);
  digitalWrite(PIN_BIG_MINUTE, LOW);
  digitalWrite(PIN_BUZZER, LOW);

  countDownTimer.attach();

  periodSevenSeg.setValue(1);

  //Make the timer run fast for testing...
  delay(KEY_DEBOUNCE_INTERVAL + 1);
  if ( ! resetBounce.read()) {
    buzzerShort();
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
  updateTimeDisplay();

  if (_buzzerOn) {
    if (millis() > _buzzerOffTime) {
      _buzzerOn = false;
      digitalWrite(PIN_BUZZER, LOW);
    }
  }

  currentDigit = (currentDigit + 1) % 8;
  scoreLeftLeft.setValue(scoreLeft.getLeftDigit());
  scoreLeftRight.setValue(scoreLeft.getRightDigit());
  scoreRightLeft.setValue(scoreRight.getLeftDigit());
  scoreRightRight.setValue(scoreRight.getRightDigit());

  scoreLeftLeft.updateDigit(currentDigit);
  scoreLeftRight.updateDigit(currentDigit);
  scoreRightLeft.updateDigit(currentDigit);
  scoreRightRight.updateDigit(currentDigit);

  smallMinute.updateDigit(currentDigit);
  bigSecond.updateDigit(currentDigit);
  smallSecond.updateDigit(currentDigit);

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

void updateTimeDisplay() {
  unsigned long timeToShow = countDownTimer.getGameTimeToShow();

  uint8_t minute10 = 0;
  uint8_t minute = 0;
  uint8_t second10 = 0;
  uint8_t second = 0;

  while (timeToShow >= 600000L) {
    minute10++;
    timeToShow = timeToShow - 600000L;
  }
  while (timeToShow >= 60000L) {
    minute++;
    timeToShow = timeToShow - 60000L;
  }
  while (timeToShow >= 10000L) {
    second10++;
    timeToShow = timeToShow - 10000L;
  }
  while (timeToShow >= 1000L) {
    second++;
    timeToShow = timeToShow - 1000L;
  }

  smallMinute.setValue(minute);
  bigSecond.setValue(second10);
  smallSecond.setValue(second);

  if (minute10 > 0) {
    digitalWrite(PIN_BIG_MINUTE, HIGH);
  } else {
    digitalWrite(PIN_BIG_MINUTE, LOW);
  }
}

void buzzerShort() {
  _buzzerOn = true;
  _buzzerOffTime = millis() + 250;
  digitalWrite(PIN_BUZZER, HIGH);
}

void buzzerShortShort() {
  _buzzerOn = true;
  _buzzerOffTime = millis() + 250;
  digitalWrite(PIN_BUZZER, HIGH);
}

void buzzerLong() {
  _buzzerOn = true;
  _buzzerOffTime = millis() + 1000;
  digitalWrite(PIN_BUZZER, HIGH);
}
