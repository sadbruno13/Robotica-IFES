import serial
import time

# configure a porta serial
porta_serial = serial.Serial('COM4', 9600) # substitua COM4 pela porta serial em que o Arduino está conectado

# envia os ângulos para o Arduino
angulo1 = 45 # ângulo do motor 1
angulo2 = 90 # ângulo do motor 2
porta_serial.write(('{} {}\n'.format(angulo1, angulo2)).encode())

# espera o Arduino finalizar a tarefa
while not porta_serial.in_waiting:
    pass
print(porta_serial.readline().decode().strip()) # imprime a resposta do Arduino

# fecha a porta serial
porta_serial.close()
