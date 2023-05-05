#include <AccelStepper.h>

// define as portas do driver A4988 conectadas ao CNC Shield
#define dirPin1 2
#define stepPin1 3
#define dirPin2 5
#define stepPin2 6

// define a velocidade e o número de passos por revolução do motor
#define velocidade 1000
#define passos_por_revolucao 200

AccelStepper stepper1(AccelStepper::DRIVER, stepPin1, dirPin1);
AccelStepper stepper2(AccelStepper::DRIVER, stepPin2, dirPin2);

void setup() {
  // inicia a comunicação serial com o PC
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    // lê os ângulos enviados pelo Python
    int angulo1 = Serial.parseInt();
    int angulo2 = Serial.parseInt();

    // calcula o número de passos para cada motor
    int passos1 = (angulo1 / 360.0) * passos_por_revolucao;
    int passos2 = (angulo2 / 360.0) * passos_por_revolucao;

    // configura os motores de passo
    stepper1.setMaxSpeed(velocidade);
    stepper1.setAcceleration(velocidade);
    stepper1.moveTo(passos1);

    stepper2.setMaxSpeed(velocidade);
    stepper2.setAcceleration(velocidade);
    stepper2.moveTo(passos2);

    // move os motores de passo
    while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) {
      stepper1.run();
      stepper2.run();
    }

    // envia a resposta para o Python
    Serial.println("Tarefa finalizada");
  }
}
