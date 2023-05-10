#include <AccelStepper.h>

// Define os pinos para o motor 1
#define motor1_passo 2
#define motor1_direcao 5

// Define os pinos para o motor 2
#define motor2_passo 3
#define motor2_direcao 6

// Define a relação entre os graus e os passos para o motor 1
#define motor1_graus_para_passos 2.222222

// Define a relação entre os graus e os passos para o motor 2
#define motor2_graus_para_passos 2.222222

// Inicializa os objetos AccelStepper para cada motor
AccelStepper stepper1(AccelStepper::DRIVER, motor1_passo, motor1_direcao);
AccelStepper stepper2(AccelStepper::DRIVER, motor2_passo, motor2_direcao);

void setup() {
  // Define as configurações iniciais dos motores
  stepper1.setMaxSpeed(10000);
  stepper1.setAcceleration(500);

  stepper2.setMaxSpeed(10000);
  stepper2.setAcceleration(500);

  Serial.begin(9600);
}

void loop() {
    // recebe os ângulos desejados para cada motor
  int angulo1 = 0;
  int angulo2 = 0;
  Serial.println("Digite o angulo para o motor 1:");
  while (angulo1 == 0) {
    if (Serial.available() > 0) {
     angulo1 = Serial.parseInt();
    }
  }
  Serial.println("Digite o número de passos para o motor 2:");
  while (angulo2 == 0) {
    if (Serial.available() > 0) {
      angulo2 = Serial.parseInt();
    }
  }
    Serial.read(); // limpa o buffer do serial

    // converte os ângulos em passos
    int passos1 = angulo1 * motor1_graus_para_passos;
    Serial.println(passos1);
    int passos2 = angulo2 * motor2_graus_para_passos;

    // calcula as velocidades proporcionais
    float velocidade_proporcional1 = 1000.0;
    float velocidade_proporcional2 = 1000.0;
    if (passos1 >= passos2) {
      velocidade_proporcional1 = velocidade_proporcional2*float((passos2/passos1));
    } else {
      velocidade_proporcional2 = velocidade_proporcional1*float((passos1/passos2));
    }

    // ajusta as velocidades para que os motores terminem o movimento juntos
    if (passos1 >= passos2) {
      stepper1.setSpeed(velocidade_proporcional1);
      stepper2.setSpeed(velocidade_proporcional2);
    } else {
      stepper1.setSpeed(velocidade_proporcional1);
      stepper2.setSpeed(velocidade_proporcional2);
    }

    // move os motores de passo
    stepper1.moveTo(passos1);
    stepper2.moveTo(passos2);

    // aguarda o término do movimento dos motores
    while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) {
      stepper1.run();
      stepper2.run();
    }
    delay(3000);
    
    // move os motores de passo para origem (0,0)
    stepper1.moveTo(0);
    stepper2.moveTo(0);
     while (stepper1.distanceToGo() != 0 || stepper2.distanceToGo() != 0) {
      stepper1.run();
      stepper2.run();
     }
  }
