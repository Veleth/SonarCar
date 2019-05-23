#include <Servo.h>
#include "TimerOne.h"
#include "Wheels.h"

#define SPEED 255
#define LEFT 0
#define RIGHT 180
// piny dla sonaru (HC-SR04)
#define TRIG A4
#define ECHO A5

// pin kontroli serwo (musi być PWM)
#define SERVO 3



Servo serwo;
Wheels w;
volatile int counter = 0;
volatile int turns = 0;

void setup() {
  pinMode(TRIG, OUTPUT);    // TRIG startuje sonar
  pinMode(ECHO, INPUT);     // ECHO odbiera powracający impuls
  w.attach(8,2,6,5,4,11);
  Serial.begin(9600);

  serwo.attach(SERVO);

/* rozejrzyj się w zakresie od 0 stopni (patrz na jedną burtę)
 *  do 180 stopni (patrz na prawą burtę). Wydrukuj na konsoli
 *  kierunek patrzenia i najbliżej widziany obiekt (pojedynczy pomiar)
 */
//  for(byte angle = 0; angle < 180; angle+= 20) {
//    lookAndTellDistance(angle);
//    delay(500);
//  }
  
  w.goForward(130);
  w.turnLeft(90);
  w.setSpeed(200);
}

void loop() { 
  if (turns < 2){
    w.forward();
    checkPath();
    delay(200);
  } else {
    w.setSpeed(200);
    while(counter > w.cntr){
      w.forward();
    }
    w.stop();
    w.turnRight(90);
    w.goForward(200);
    delay(100000);
  }
}

void checkPath(){
  int distanceLeft = lookAndTellDistance(LEFT);
  #sendSignal
  if(lookAndTellDistance(80) <= 70 || lookAndTellDistance(100) <= 70){
    w.stop();
  }
  int distanceRight = lookAndTellDistance(RIGHT);
  #sendSignal
}

int lookAndTellDistance(byte angle) {
  
  unsigned long tot;      // czas powrotu (time-of-travel)
  unsigned int distance;

  serwo.write(angle);
  delay(100);  
/* uruchamia sonar (puls 10 ms na `TRIGGER')
 * oczekuje na powrotny sygnał i aktualizuje
 */
  digitalWrite(TRIG, HIGH);
  delay(10);
  digitalWrite(TRIG, LOW);
  tot = pulseIn(ECHO, HIGH);
  

/* prędkość dźwięku = 340m/s => 1 cm w 29 mikrosekund
 * droga tam i z powrotem, zatem:
 */
  distance = tot/58;
  return distance;
}
