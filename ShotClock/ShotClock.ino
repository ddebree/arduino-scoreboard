#include <SPI.h>
#include <NRFLite.h>

const static uint8_t RADIO_ID = 23;       // Our radio's id.  The transmitter will send to this id.
const static uint8_t PIN_RADIO_CE = A0;
const static uint8_t PIN_RADIO_CSN = A1;

//If we havent updated the display in X millis, turn the display off
const static unsigned long SLEEP_TIME = 90000;

struct RadioPacket {
    boolean clockRunning;
    boolean clockVisible;
    unsigned long currentTime;
};

NRFLite radio;
RadioPacket radioData;

//The last time the loop function was started:
unsigned long lastLoopStartTime = 0;

//The last time the time was changed on the display:
unsigned long lastChangeTime = 0;

void setup() {
  radioData.clockRunning = true;
  radioData.clockVisible = true;
  radioData.currentTime = 100000;
  
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);

  Serial.begin(9600);

  if ( ! radio.init(RADIO_ID, PIN_RADIO_CE, PIN_RADIO_CSN)) {
    Serial.println("Cannot communicate with radio");
    while (1); // Wait here forever.
  }
}

void loop() {
  unsigned long loopStartTime = millis();
  
  //Receive the current time over the radio:
  if (radio.hasData()){
    radio.readData(&radioData); 
    lastChangeTime = millis();
    
    Serial.print("Received: ");
    Serial.print(radioData.currentTime);
    Serial.print(",");
    Serial.print(radioData.clockRunning);
    Serial.print(",");
    Serial.print(radioData.clockVisible);
    Serial.println();
  } else if (radioData.clockRunning) {
    lastChangeTime = millis();
    unsigned long loopTime = loopStartTime - lastLoopStartTime;
    if (radioData.currentTime < loopTime) {
      radioData.currentTime = 0;
      radioData.clockRunning = false;
      Serial.print("Self updating the time hit zero!");
    } else {
      radioData.currentTime -= loopTime;
      Serial.print("Self updating the time. Current time=");
      Serial.println(radioData.currentTime);
    }
  } else {
    Serial.print("Clock not running. Current time=");
    Serial.println(radioData.currentTime);
  }

  //Save the time the previous loop started in case we want to auto update the currentTime
  lastLoopStartTime = loopStartTime;
  
  if (radioData.clockVisible) {
    unsigned long currentTimeInSeconds = radioData.currentTime / 1000;
    byte value = currentTimeInSeconds % 100;
    byte digit10 = value / 10;
    byte digit1 = value % 10;

    digitalWrite(3, (digit10 & 0x01) > 0 ? HIGH : LOW);
    digitalWrite(4, (digit10 & 0x08) > 0 ? HIGH : LOW);
    digitalWrite(5, (digit10 & 0x04) > 0 ? HIGH : LOW);
    digitalWrite(6, (digit10 & 0x02) > 0 ? HIGH : LOW);
  
    digitalWrite(7, (digit1 & 0x01) > 0 ? HIGH : LOW);
    digitalWrite(8, (digit1 & 0x08) > 0 ? HIGH : LOW);
    digitalWrite(9, (digit1 & 0x04) > 0 ? HIGH : LOW);
    digitalWrite(10, (digit1 & 0x02) > 0 ? HIGH : LOW);
  } else {
    //This is us trying to get 8+4 to show on the display. The driver just shows blank. Great for power saving...
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);

    digitalWrite(9, HIGH);
    digitalWrite(10, HIGH);
  }

  unsigned long timeSinceLastChange = millis() - lastChangeTime;
  if ( timeSinceLastChange > SLEEP_TIME) {
    radioData.clockVisible = false;
  }
}
