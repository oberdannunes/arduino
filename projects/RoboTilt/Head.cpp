#include <Arduino.h>
#include "Head.h"

Head::Head() {
}

Head::~Head() {
    free(_ultrassonic);
}

void Head::attach(int pinH, int pinV, int pinUT, int pinUE) {
    _headH.attach(pinH);
    _headV.attach(pinV);

    _ultrassonic = new HCSR04(pinUT, pinUE);
}

void Head::moveHead(int h, int v) {
    _headH.write(h);
    _headV.write(v);
}

void Head::center() {
    moveHead(90, 50);
}

float Head::readDistance() {
    return _ultrassonic->dist();
}