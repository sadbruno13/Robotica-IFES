#include <AccelStepper.h>

// define as portas do driver A4988 conectadas ao CNC Shield
#define dirPin1 2
#define stepPin1 3
#define dirPin2 5
#define stepPin2 6

AccelStepper stepper1(AccelStepper::DRIVER, stepPin1, dirPin1);
AccelStepper stepper2(AccelStepper::DRIVER, stepPin2, dirPin2);

void setup() {
  // inicia a comunicação serial com o PC
  stepper1.setMaxSpeed(10000);
  stepper2.setMaxSpeed(10000);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    // lê os ângulos enviados pelo Python
    int vel_m1 = Serial.parseInt();
    int vel_m2 = Serial.parseInt();
    int accel = Serial.parseInt();
    int passosm1 = Serial.parseInt();
    int passosm2 = Serial.parseInt();

    // configura os motores de passo
    stepper1.setSpeed(vel_m1);
    stepper1.setAcceleration(accel);
    stepper1.moveTo(passosm1);

    stepper2.setSpeed(vel_m2);
    stepper2.setAcceleration(accel);
    stepper2.moveTo(passosm2);

    // move os motores de passo
    while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) {
      stepper1.run();
      stepper2.run();
    }

    delay(3000);
    stepper1.moveTo(0);
    stepper2.moveTo(0);

    while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) {
      stepper1.run();
      stepper2.run();
    }

    // envia a resposta para o Python
    Serial.println("Tarefa finalizada");
  }
  Serial.read(); // limpa o buffer do serial
}
