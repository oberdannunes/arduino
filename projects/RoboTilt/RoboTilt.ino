#include <Arduino.h>
#include <MsTimer2.h>
#include "Head.h"

#define PINHEADH 8
#define PINHEADV 9

Head head;
float distance;

void setup() {
    // put your setup code here, to run once:
    //Serial.begin(9600);
    
    //Atacha e centraliza a cabeça
    head.attach(PINHEADH, PINHEADV, 2, 3);
    head.center();

    MsTimer2::set(50, readCurrentDistance);
    MsTimer2::start();
}



void loop() {
  // put your main code here, to run repeatedly:

  if (distance < 30) {
    head.moveHead(90, 30);
  } else {
    head.center();
  }

}

void readCurrentDistance() {
  distance = head.readDistance();
  //Serial.println(distance);
}




