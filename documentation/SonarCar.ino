#include <Servo.h>
#include <SoftwareSerial.h>
#include "TimerOne.h"
#include "Wheels.h"

// constants
#define SPEED 150
#define LEFT 180
#define RIGHT 0
#define FRONT 90
#define TOL 200
#define DEGS 20
#define MAX 4000
#define SMALLSTEP 25
// piny dla sonaru (HC-SR04)
#define TRIG A4
#define ECHO A5

// pin kontroli serwo (musi być PWM)
#define SERVO 5

SoftwareSerial BTSerial(10, 9);
Servo serwo;
Wheels w;
volatile long counter = 0;
volatile int turns = 0;
int i = 0;


void setup() {
  pinMode(TRIG, OUTPUT);    // TRIG startuje sonar
  pinMode(ECHO, INPUT);     // ECHO odbiera powracający impuls
  w.attach(8, 13, 4, 6, 7, 12); //8 Green RF, 11 Blue RB, 4 Yellow RS, 6 Gray LB, 7 Purple LB, 12 White LS
  //pRF, pRB, pRS, pLF, pLB, pLS
  BTSerial.begin(9600);
  serwo.attach(SERVO);
  bool flag = false;
  while(!flag){
    if(BTSerial.available()){
      flag = true;
    }
    delay(500);
  }
  w.setSpeed(SPEED);
  w.forward();
}
/*
   Returns the distance in mm at angle angle
*/
int lookAndTellDistance(byte angle) {
  unsigned long tot;
  unsigned int distance;
  
  serwo.write(angle);
  delay(300);

  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
  tot = pulseIn(ECHO, HIGH);

  distance = 10 * (tot / 58); 
  Serial.write(distance); 
  return distance; //mm
}

void loop() {

    w.stop();
    int distanceLeft = lookAndTellDistance(LEFT);
    if(distanceLeft > MAX){ //repeat if over limit
      w.forward();
      delay(SMALLSTEP);
      w.stop();
      int distanceLeft = lookAndTellDistance(LEFT);
    }
    BTSerial.print("<L;");
    BTSerial.print(distanceLeft);
    BTSerial.print(";");
    BTSerial.print(w.cntr);
    BTSerial.print(">");

    if (lookAndTellDistance(FRONT+DEGS) <= TOL || lookAndTellDistance(FRONT) <= TOL || lookAndTellDistance(FRONT-DEGS) <= TOL) {
      w.stop();
      exit(0);
    }

    w.stop();
    int distanceRight = lookAndTellDistance(RIGHT);
    if(distanceRight > MAX){ //repeat if over limit
      w.forward();
      delay(SMALLSTEP);
      w.stop();
      int distanceRight = lookAndTellDistance(RIGHT);
    }
    BTSerial.print("<R;");
    BTSerial.print(distanceRight);
    BTSerial.print(";");
    BTSerial.print(w.cntr);
    BTSerial.print(">");

    if (lookAndTellDistance(FRONT-DEGS) <= TOL || lookAndTellDistance(FRONT) <= TOL || lookAndTellDistance(FRONT+DEGS) <= TOL) {
      w.stop();
      exit(0);
    }

    w.forward();
    delay(100);
}
