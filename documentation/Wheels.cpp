//#include <Arduino.h>
//#include "TimerOne.h"
#include "Wheels.h"

#define SET_MOVEMENT(side,f,b) digitalWrite( side[0], f);\
                               digitalWrite( side[1], b)
long long cnt;
int period = 5000;
int speedLeft = 0;
int signLeft = 1;
int speedRight = 0;
int signRight = 1;


void cntplus(){
  cnt++;
}


Wheels::Wheels() 
{
  cnt = 0;
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), cntplus, CHANGE); //TODO:Check
  attachInterrupt(digitalPinToInterrupt(3), cntplus, CHANGE); //TODO:Check

 }

void zeroCounters(){
  cnt=0;
}

void Wheels::attachRight(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsRight[0] = pF;
    this->pinsRight[1] = pB;
    this->pinsRight[2] = pS;
}


void Wheels::attachLeft(int pF, int pB, int pS)
{
    pinMode(pF, OUTPUT);
    pinMode(pB, OUTPUT);
    pinMode(pS, OUTPUT);
    this->pinsLeft[0] = pF;
    this->pinsLeft[1] = pB;
    this->pinsLeft[2] = pS;
}

void Wheels::setSpeedRight(uint8_t s)
{
    analogWrite(this->pinsRight[2], s);
    speedRight = s;
}

void Wheels::setSpeedLeft(uint8_t s)
{
    analogWrite(this->pinsLeft[2], s);
    speedLeft = s;
}

void Wheels::setSpeed(uint8_t s)
{
    zeroCounters();
    setSpeedLeft(s);
    setSpeedRight(s);
}

void Wheels::attach(int pRF, int pRB, int pRS, int pLF, int pLB, int pLS)
{
    this->attachRight(pRF, pRB, pRS);
    this->attachLeft(pLF, pLB, pLS);
}

void Wheels::forwardLeft() 
{
    SET_MOVEMENT(pinsLeft, HIGH, LOW);
    signLeft = 1;
}

void Wheels::forwardRight() 
{
    SET_MOVEMENT(pinsRight, HIGH, LOW);
    signRight = 1;
}

void Wheels::backLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, HIGH);
    signLeft = -1;
}

void Wheels::backRight()
{
    SET_MOVEMENT(pinsRight, LOW, HIGH);
    signRight = -1;
}

void Wheels::forward()
{
    this->cntr = cnt;
    this->forwardLeft();
    this->forwardRight();
}

void Wheels::back()
{
    this->backLeft();
    this->backRight();
}

void Wheels::stopLeft()
{
    SET_MOVEMENT(pinsLeft, LOW, LOW);
}

void Wheels::stopRight()
{
    SET_MOVEMENT(pinsRight, LOW, LOW);
}

void Wheels::stop()
{
  this->cntr = cnt;
    this->stopLeft();
    this->stopRight();
}

void Wheels::goForward(int cm){
    int ocnt = cnt;
    this->setSpeed(150);
    this->forward();
    while (cnt-ocnt < 4*cm) delay(50);
    this->stop();
}

void Wheels::goBack(int cm){
    this->setSpeed(150);
    this->back();
    while (cnt < 8*cm) delay(50);
    this->stop();
    zeroCounters();
}

void Wheels::turnLeft(int dgrs){
  zeroCounters();
  this->setSpeed(0);
  this->setSpeedRight(150);
  this->forward();
  while (cnt < 1.8*dgrs)delay(50);
  this->stop();
  zeroCounters();
}

void Wheels::turnRight(int dgrs){
  zeroCounters();
  this->setSpeed(0);
  this->setSpeedLeft(150);
  this->forward();
  while (cnt < 1.8*dgrs) delay(50);
  this->stop();
  zeroCounters();
}
