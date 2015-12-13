#include <Wire.h>

void setup() {
  Wire.begin();
}

byte deviceId = 0;

void loop() {
  Wire.beginTransmission(deviceId + 0b0100000); // transmit to device deviceId
  Wire.write("x is ");        // sends five bytes
  Wire.write(12);              // sends one byte
  Wire.endTransmission();    // stop transmitting

  delay(500);

}
