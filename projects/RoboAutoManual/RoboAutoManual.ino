#define BLYNK_PRINT Serial
#include <BlynkSimpleSerialBLE.h>
//#include <BlynkSimpleEthernet.h>
#include "Ultrasonic.h"
#include <SoftwareSerial.h>
#include <SimpleTimer.h>
#include <AFMotor.h>
#include <Servo.h>

#define ultrassonicTrigger 14
#define ultrassonicEcho 15

char blynkAuth[] = "711548f74bee4e239256c84316168e44";

Ultrasonic ultrassonic(ultrassonicTrigger, ultrassonicEcho); 
SoftwareSerial bluetooth(19, 18); // RX, TX
Servo head;
WidgetTerminal terminal(V3);

AF_DCMotor motorR(3);
AF_DCMotor motorL(4);

float distance = 0;
int x, y;
bool turbo;
bool autoMode;

void setup() {
  //Set console baud rate
  Serial.begin(9600); 

  Serial.println("####### INITIATING ROBOT #######");

  Serial.println("");
  delay(100);
  
  Serial.println("       #   #       ");
  Serial.println("         |         ");
  Serial.println("       \\___/       ");
  
  Serial.println("");
  delay(100);

  Serial.println("1) Initiating ultrassonic sensor...");
  distance = ultrassonic.distanceRead();
  Serial.print("\tcurrent distance: ");
  Serial.print(distance);
  Serial.println(" (cm)");

  Serial.println(""); 
  delay(100);

  Serial.println("2) Initiating bluetooth module...");
  bluetooth.begin(57600);

  Serial.println(""); 
  delay(100);

  Serial.println("3) Initiating blynk module...");
  Blynk.begin(blynkAuth, bluetooth);

  Serial.println(""); 
  delay(100);

  Serial.println("4) Initiating servo motor...");
  head.attach(10);

  Serial.println(""); 
  delay(100);
  
  terminal.println("I'M READY!! :)");

  x = 128, y = 128;
  head.write(90);
  autoMode = false;
  turbo = false;

  LogControlMode();
  LogSpeedMode();
}

void loop() {
  Blynk.run();

  if (autoMode) {
    







    
  }
}


//Joystick
BLYNK_WRITE(V1) {
  if (autoMode) return;
  
  x = param[0].asInt(); 
  y = param[1].asInt(); 
  updateDirection();  
}

//Botão auto/manual
BLYNK_WRITE(V2) {
  autoMode = param[0].asInt();
  LogControlMode();
}

//Botão turbo
BLYNK_WRITE(V0) {
  if (autoMode) return;

  int novo = param.asInt();

  if (turbo == 0 && novo == 1) {
    motorL.setSpeed(0);   motorR.setSpeed(0);
    motorL.run(FORWARD);  motorR.run(FORWARD);
    motorL.setSpeed(255); motorR.setSpeed(255);
    delay(50);
    motorL.run(BACKWARD); motorR.run(BACKWARD);
    delay(50);
    motorL.run(FORWARD);  motorR.run(FORWARD);
    delay(50);
    motorL.run(BACKWARD); motorR.run(BACKWARD);
    delay(20);
    motorL.run(FORWARD);  motorR.run(FORWARD);
    delay(20);
    motorL.setSpeed(0);   motorR.setSpeed(0);
    motorL.run(RELEASE);  motorR.run(RELEASE);
  }

  turbo = novo;
  LogSpeedMode();
}

void LogControlMode() {
  terminal.print("CONTROL: ");
  terminal.println(autoMode ? "Auto" : "Manual");
  terminal.flush();
}

void LogSpeedMode() {
  terminal.print("SPEED: ");
  terminal.println(turbo ? "Turbo" : "Normal");
  terminal.flush();
}

void updateDirection() {

  if (y > 130) {
    motorL.run(FORWARD);
    motorR.run(FORWARD);
  } else if (y < 110) {
    motorL.run(BACKWARD);
    motorR.run(BACKWARD);
  } else {
    motorL.run(RELEASE);
    motorR.run(RELEASE);
  }

  int velocity;

  if (y > 128) {
    velocity = y;
  } else {
    velocity = 255 - y;
  }

  int velocityL = velocity;
  int velocityR = velocity;

  if (x > 128) {
    velocityR -= (x - 128);
  } else if (x < 128) {
    velocityL -= (128 - x);
  }

  if (turbo == 0) {
    velocityL *= 0.6;
    velocityR *= 0.6;
  } else {
    velocityL *= 1;
    velocityR *= 1;

    if (velocityL > 255) velocityL = 255;
    if (velocityR > 255) velocityR = 255;
  }

//  Serial.print("L: ");
//  Serial.println(velocityL);
//  
//  Serial.print("R: ");
//  Serial.println(velocityR);

  motorL.setSpeed(velocityL);
  motorR.setSpeed(velocityR);
}
