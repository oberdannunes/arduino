

#define tempoMaximoDeUmaPalma  150 //milisegundos
#define tempoMaximoEntrePalmas 500 //milisegundos

long tempoEspera = 0;
long tempoEsperaEntrePalmas = 0;
int contaPalmas = 0;

void executaAcao();

void setup() {
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  pinMode(12, OUTPUT);
  pinMode(7, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorSom = digitalRead(7);
  
  if (sensorSom == 0) {
    Serial.print("contaPalmas = ");
    Serial.print(contaPalmas);
    Serial.print('\n');

    if (tempoEspera == 0) {
      tempoEspera = tempoEsperaEntrePalmas = millis();
      contaPalmas++;
    } else if ((millis() - tempoEspera) >= tempoMaximoDeUmaPalma) {
      tempoEspera = 0;
    }

    //digitalWrite(12, estadoLed);
  }

  if (contaPalmas != 0 && millis() - tempoEsperaEntrePalmas >= 500) {
    executaAcao();
    contaPalmas = 0;
    delay(100); 
    tempoEsperaEntrePalmas = millis();
  }
}

void executaAcao() {
  if (contaPalmas == 1) {
    digitalWrite(12, 1);
  } else if (contaPalmas == 2) {
    digitalWrite(12, 0);
  }
}
