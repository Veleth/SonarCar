#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 9); // RX, TX

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() { // run over and over
// Read the output of the HC-05 and send to the serial monitor of the Arduino 
  if (BTSerial.available()){
    Serial.write(BTSerial.read());
  }

  // Read what was typed on the serial monitor of the Arduino and send to the HC-05
  if (Serial.available()){
    BTSerial.write(Serial.read());
  }
}
