#include <Servo.h>
#include <AFMotor.h>
#include "Ultrasonic.h"

#define pinEcho 3
#define pinTrigger 4

#define AHEAD 0
#define RIGHT 1
#define LEFT 2
#define STOP 3

#define speedL 90
#define speedM 150
#define speedH 255

AF_DCMotor motorR(3);
AF_DCMotor motorL(4);
Servo head;
Ultrasonic ultrasonic(pinTrigger, pinEcho);

float distance = 0;
float limite = 40;
int aheadInteraction = 0;
int lastTurn180 = LEFT;

void setup() {
  head.attach(10);
  head.write(90);

  distance = readDistance();
  distance = readDistance();
  distance = readDistance();
  delay(100);
}

void loop() {
  distance = readDistance();

  if (distance > limite) {
    aheadInteraction ++;
    move(AHEAD, speedL, 0);

  } else {
    move(STOP, 0, 0);

    aheadInteraction = 0;
    look(LEFT);
    delay(500);
    distance = readDistance();

    if (distance > limite) {
      look(AHEAD);
      move(LEFT, speedM, 500);


    } else {
      look(RIGHT);
      delay(500);
      distance = readDistance();

      if (distance > limite) {
        move(RIGHT, speedM, 500);
        look(AHEAD);

      } else {
        shakeHead();
        delay(500);

        lastTurn180 = lastTurn180 == LEFT ? RIGHT : LEFT;

        look(AHEAD);
        move(lastTurn180, speedM, 900);
      }

    }

    delay(200);
  }

  delay(100);
}

float readDistance() {
  //Lendo o sensor
  long microsec = ultrasonic.timing();

  //Convertendo a distância em CM
  return ultrasonic.convert(microsec, Ultrasonic::CM);
}

void look(int direction) {
  int angle = 90;

  if (direction == LEFT) {
    angle = 135;
  } else if (direction == RIGHT) {
    angle = 45;
  }

  head.write(angle);
}

void move(int direction, int speed, int time) {
  if (direction == AHEAD) {
    motorL.run(RELEASE);
    motorR.run(RELEASE);
    motorL.setSpeed(0);
    motorR.setSpeed(0);
    motorL.run(FORWARD);
    motorR.run(FORWARD);
    motorL.setSpeed(speed);
    motorR.setSpeed(speed);
  } else if (direction == RIGHT) {
    motorL.run(FORWARD);
    motorR.run(RELEASE);
    delay(time);
    motorL.run(BACKWARD);
    delay(100);
    motorL.run(RELEASE);
    motorR.run(RELEASE);
  } else if (direction == LEFT) {
    motorL.run(RELEASE);
    motorR.run(FORWARD);
    delay(time);
    motorR.run(BACKWARD);
    delay(100);
    motorL.run(RELEASE);
    motorR.run(RELEASE);
  } else if (direction == STOP) {
    motorL.run(RELEASE);
    motorR.run(RELEASE);
  }

  if (direction != STOP) {
    motorL.setSpeed(speed);
    motorR.setSpeed(speed);
  }

  //  if (direction == LEFT || direction == RIGHT) {
  //    delay(time);
  //    motorL.run(RELEASE);
  //    motorR.run(RELEASE);
  //  }

}

void shakeHead() {
  float angle = head.read();

  int step = 20;
  int duration = 100;

  head.write(angle + step);
  delay(duration);
  head.write(angle - step);
  delay(duration);
  head.write(angle + step);
  delay(duration);
  head.write(angle);
}

