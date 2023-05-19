#include <AccelStepper.h>

// Definindo as constantes
const float L1 = 10.0; // Comprimento do braço 1 em cm
const float L2 = 10.0; // Comprimento do braço 2 em cm

// Definindo os objetos AccelStepper para controlar os motores
AccelStepper motor1(AccelStepper::DRIVER, 2, 5); // Motores conectados aos pinos 2 e 3
AccelStepper motor2(AccelStepper::DRIVER, 3, 6);

void setup() {
  Serial.begin(9600); // Inicializando a comunicação serial
  while (!Serial); // Aguardando até que a porta serial esteja aberta

  // Configurando as propriedades dos motores
  motor1.setMaxSpeed(1000);
  motor1.setAcceleration(500);
  motor2.setMaxSpeed(1000);
  motor2.setAcceleration(500);

  // Movendo os motores para a posição inicial (ângulo zero)
  motor1.moveTo(0);
  motor2.moveTo(0);
}

void loop() {
  // Lendo as coordenadas do ponto a ser alcançado
  float x, y;
  Serial.println("Digite as coordenadas x e y do ponto a ser alcançado:");
  while (Serial.available() < 2); // Aguarda até que as duas coordenadas sejam digitadas
  x = Serial.parseFloat();
  y = Serial.parseFloat();

  // Calculando os ângulos usando a cinemática inversa
  float theta1, theta2;
  calcularCinematicaInversa(x, y, L1, L2, &theta1, &theta2);

  // Convertendo os ângulos para passos e movendo os motores para as posições correspondentes
  long passos1 = theta1 * (200.0 / M_PI); // 200 passos por volta
  long passos2 = theta2 * (200.0 / M_PI);
  motor1.moveTo(passos1);
  motor2.moveTo(passos2);

  // Aguardando até que os motores atinjam as posições desejadas
  while (motor1.distanceToGo() != 0 || motor2.distanceToGo() != 0) {
    motor1.run();
    motor2.run();
  }
}

// Função para calcular a cinemática inversa
void calcularCinematicaInversa(float x, float y, float L1, float L2, float* theta1, float* theta2) {
  // Calcula os ângulos usando a fórmula da cinemática inversa
  float r = sqrt(x * x + y * y);
  *theta2 = acos((r * r - L1 * L1 - L2 * L2) / (2 * L1 * L2));
  *theta1 = atan2(y, x) - atan2(L2 * sin(*theta2), L1 + L2 * cos(*theta2));

  // Escolhe o menor ângulo possível para cada um dos ângulos theta1 e theta2
  if (*theta1 > M_PI) {
    *theta1 -= 2 * M_PI;
  } else if (*theta1 < -M_PI) {
    *theta1 += 2 * M_PI;
  }

 if (*theta2 > M_PI) {
    *theta2 -= 2 * M_PI;
  } else if (*theta2 < -M_PI) {
    *theta2 += 2 * M_PI;
  }
}