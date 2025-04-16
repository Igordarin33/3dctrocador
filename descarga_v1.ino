#include <Servo.h>

Servo meuServo;
const int pinoServo = 3;
const int pinoSensor = 2;

void setup() {
  pinMode(pinoSensor, INPUT_PULLUP);
  meuServo.attach(pinoServo);
  meuServo.write(63); // Começa fechado
}

void loop() {
  int estado = digitalRead(pinoSensor);

  if (estado == LOW) {
    // 24V ligado → ABRE a alavanca
    meuServo.write(0);
  } else {
    // 24V desligado → FECHA a alavanca
    meuServo.write(63);
  }

  delay(100);
}
