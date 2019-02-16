#include <Arduino.h>
#include <MsTimer2.h>
#include "Head.h"
#include <HampelFilter.h>

#define PINHEADH 8
#define PINHEADV 9

Head head;
float distance;
HampelFilter distanceFilter = HampelFilter(0.00, 3, 5.50);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    
    //Atacha e centraliza a cabeça
    head.attach(PINHEADH, PINHEADV, 2, 3);
    head.center();

    MsTimer2::set(100, readCurrentDistance);
    MsTimer2::start();
}



void loop() {
  // put your main code here, to run repeatedly:


}

void readCurrentDistance() {
  float reading = head.readDistance();

  distanceFilter.write(reading);

  bool outlier = false;
  if (!distanceFilter.checkIfOutlier(reading)) {
    distance = reading;
  } else {
    outlier =true;
  }

    if (distance < 20) {
    head.moveHead(90, 30);
  } else {
    head.center();
  }

  //Serial.print(distance);
  //Serial.print(" - ");
  //Serial.println(outlier);

  
}




