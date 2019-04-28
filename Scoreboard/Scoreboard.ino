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
#include "buzzer.h"

DownTimer countDownTimer;
Score scoreLeft;
Score scoreRight;
Buzzer buzzer;

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

struct RadioPacket {
    boolean clockRunning;
    boolean clockVisible;
    unsigned long currentTime;
};

NRFLite radio;
RadioPacket radioData;
unsigned long nextRadioSendTime = 0;
bool radioSendNow = false;

void setup() {
  setupKeys();
  setupDisplays();

  scoreLeft.init(false);
  scoreRight.init(true);
  countDownTimer.init();

  buzzer.init();
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  //Make the timer run fast for testing...
  delay(KEY_DEBOUNCE_INTERVAL + 1);
  if ( ! resetBounce.read()) {
    buzzer.buzzerShort();
    countDownTimer.setFastTime();
  }
  //Some debugging support:
  Serial.begin(9600);
  if ( ! radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN)) {
    Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.
  }
  Serial.println("Setup done!");
}

void loop() {
  //Serial.println("Loop Start");
  updateKeys();
  updateTimeDisplay();
  updateScoreDisplay();
  updateBuzzer();
  updateRadio();
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
    countDownTimer.resetShotClock();
  }
}

void alternatePeriod() {
  if (periodSevenSeg.getValue() <= 1) {
    periodSevenSeg.setValue(2);
  } else {
    periodSevenSeg.setValue(1);
  }
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

void updateScoreDisplay() {
  scoreLeftLeft.setValue(scoreLeft.getLeftDigit());
  scoreLeftRight.setValue(scoreLeft.getRightDigit());
  scoreRightLeft.setValue(scoreRight.getLeftDigit());
  scoreRightRight.setValue(scoreRight.getRightDigit());
}

void updateBuzzer() {
  if (buzzer.isBuzzerOn()) {
    digitalWrite(PIN_BUZZER, HIGH);
  } else {
    digitalWrite(PIN_BUZZER, LOW);
  }
}

void updateRadio() {
  //We only send the radio data occasionally:
  if ((millis() > nextRadioSendTime) || radioSendNow) {
    Serial.println("Sending radio data");
    radioSendNow = false;
    unsigned long elapsedShotTime = countDownTimer.getShotTimeToShow();
    if (elapsedShotTime < 60000L) {
      radioData.clockRunning = countDownTimer.isRunning();
      radioData.clockVisible = true;
      radioData.currentTime = 60000L - elapsedShotTime;
    } else {
      radioData.clockRunning = false;
      radioData.clockVisible = true;
      radioData.currentTime = 0;
    }

    radio.send(RADIO_ID, &radioData, sizeof(radioData));
    nextRadioSendTime = millis() + RADIO_SEND_INTERVAL;
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

void setupDisplays() {
  //Attach the seven segment drivers:
  scoreLeftLeft.attach(SCORE_LEFT_BIG);
  scoreLeftRight.attach(SCORE_LEFT_SMALL);
  scoreRightLeft.attach(SCORE_RIGHT_BIG);
  scoreRightRight.attach(SCORE_RIGHT_SMALL);
  periodSevenSeg.attach(PERIOD);
  smallMinute.attach(TIME_MINUTE_SMALL);
  bigSecond.attach(TIME_SECOND_BIG);
  smallSecond.attach(TIME_SECOND_SMALL);

  periodSevenSeg.setValue(1);

  pinMode(PIN_DOTS, OUTPUT);
  digitalWrite(PIN_DOTS, HIGH);
  pinMode(PIN_BIG_MINUTE, OUTPUT);
  digitalWrite(PIN_BIG_MINUTE, LOW);
}
