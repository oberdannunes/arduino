#include <BlynkSimpleSerialBLE.h>
#include "Ultrasonic.h"
#include <SoftwareSerial.h>
#include <SimpleTimer.h>
#include <AFMotor.h>
#include <Servo.h> 

char auth[] = "d98a351f452c4b48af5b3091dfa256b2";

#define pinTrigger 14
#define pinEcho 15

//Configura os pinos utilizados para comunicação com o módulo bluetooth
SoftwareSerial bluetooth(18, 19); // RX, TX

//Configura os pinos para o sensor ultrassônico
Ultrasonic ultrasonic(pinTrigger, pinEcho);
float distance = 0;

//SimpleTimer timerDistance;

//Configura motores de passo para os motores esquerdo e direito
AF_DCMotor motorR(3);
AF_DCMotor motorL(4);

Servo head;

int x, y;
int turbo = 0;

void setup() {
  //Set console baud rate
  //Serial.begin(9600); 
  //delay(10);
  
  bluetooth.begin(57600);
  delay(10);


  //Conecta no blynk
  Blynk.begin(auth, bluetooth);

  head.attach(10);

  head.write(5);

 // Blynk.virtualWrite(0, distance);
 // timerDistance.setInterval(1000, readDistance);

  //Define os valores iniciais para x e y, para bater com a posição inicial do joystick
  x = 128, y = 128;
}

void loop() {
  //Executa as rotinas do Blynk
  Blynk.run();

 // timerDistance.run();
  //delay(100);
}

void readDistance() {
  //Lendo o sensor
  long microsec = ultrasonic.timing();

  //Convertendo a distância em CM
  float distance = ultrasonic.convert(microsec, Ultrasonic::CM);

  Blynk.virtualWrite(10, distance);
}

//Disparado quando o botão turbo é pressionado
//Button Widget is writing to pin V1
BLYNK_WRITE(V0) {
  int novo = param.asInt();

  if (turbo == 0 && novo == 1) {
    motorL.setSpeed(0);
    motorR.setSpeed(0);
    motorL.run(FORWARD);
    motorR.run(FORWARD);
    motorL.setSpeed(255);
    motorR.setSpeed(255);
    delay(50);
    motorL.run(BACKWARD);
    motorR.run(BACKWARD);
    delay(50);
    motorL.run(FORWARD);
    motorR.run(FORWARD);
    delay(50);
    motorL.run(BACKWARD);
    motorR.run(BACKWARD);
    delay(20);
    motorL.run(FORWARD);
    motorR.run(FORWARD);
    delay(20);
    motorL.setSpeed(0);
    motorR.setSpeed(0);
    motorL.run(RELEASE);
    motorR.run(RELEASE);
  }

  turbo = novo;
}

//Joystick acionado
BLYNK_WRITE(V1) {
  x = param[0].asInt(); 
  y = param[1].asInt(); 
  updateDirection();  
  //Serial.write("aaa");
}

//Botão auto/manual acionado
BLYNK_WRITE(V2) {
  bool modo_auto = param.asInt() == 1;
  
}

//Rotina responsável por mover o robô de acordo com a posição do joystick
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

