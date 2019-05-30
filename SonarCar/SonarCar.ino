#include <Servo.h>
#include <SoftwareSerial.h>
#include "TimerOne.h"
#include "Wheels.h"

// constants
#define SPEED 255
#define LEFT 0
#define RIGHT 180
#define FRONT 90
// piny dla sonaru (HC-SR04)
#define TRIG A4
#define ECHO A5

// pin kontroli serwo (musi być PWM)
#define SERVO 3

SoftwareSerial BTSerial(10, 9);
Servo serwo;
Wheels w;
volatile long counter = 0;
volatile int turns = 0;
short distanceLeft = 0;
short distanceRight = 0;

void setup() {
  pinMode(TRIG, OUTPUT);    // TRIG startuje sonar
  pinMode(ECHO, INPUT);     // ECHO odbiera powracający impuls
  w.attach(8, 2, 6, 5, 4, 11); //8 Green RF, 11 Blue RB, 4 Yellow RS, 5 Purple LF, 6 Gray LB, 12 White LS
  //pRF, pRB, pRS, pLF, pLB, pLS
  Serial.begin(9600);
  serwo.attach(SERVO);

  w.setSpeed(150);
  w.forward();
}
/*
   Returns the distance in mm at angle angle
*/
int lookAndTellDistance(byte angle) {
  unsigned long tot;
  unsigned int distance;

  serwo.write(angle);
  delay(100);

  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
  tot = pulseIn(ECHO, HIGH);

  distance = 100 * tot / 58;
  return distance; //mm
}

void loop() {
  //  digitalWrite(12, LOW);
  //  digitalWrite(5, LOW);
  //  digitalWrite(6, HIGH);
  //  digitalWrite(5, HIGH);
  //  delay(1000);
  //  digitalWrite(6, LOW);
  //  digitalWrite(5, LOW);
  //  digitalWrite(6, HIGH);
  //  digitalWrite(12, HIGH);
  //  delay(5000);
  //  digitalWrite(6, LOW);
  //  digitalWrite(12, LOW);
  //  digitalWrite(12, HIGH);
  //  digitalWrite(5, HIGH);
  //  delay(15000);
  if (BTSerial.available()) {
    distanceLeft = lookAndTellDistance(LEFT);
    BTSerial.write("<L;" + distanceLeft + ';' + w.cntr + '>');
    if (lookAndTellDistance(80) <= 70 || lookAndTellDistance(100) <= 70) {
      w.stop();
      exit(0);
    }

    distanceRight = lookAndTellDistance(RIGHT);
    BTSerial.write("<R;" + distanceRight + ';' + w.cntr + '>');
    if (lookAndTellDistance(80) <= 70 || lookAndTellDistance(100) <= 70) {
      w.stop();
      exit(0);
    }
  }
  else {
    w.stop();
    delay(500);
    w.forward();
  }
}
