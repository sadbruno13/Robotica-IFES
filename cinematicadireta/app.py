import serial
import math

passos_motor_completo = 800
def ang_to_passos(angulo):
    passos = angulo * (passos_motor_completo/360)
    return passos

def get_vel_motor(vel_init, accel, angulo_m1, angulo_m2):

    if(angulo_m1>angulo_m2):
        vel_m1 = vel_init
        accel_m1 = accel
        accel_m2 = accel_m1

        delta = vel_m1**2 - 4*(accel_m1/2)*(-ang_to_passos(angulo_m1))
        t_m1 = (-vel_m1 + math.sqrt(delta))/(2*(accel_m1/2))
        vel_m2 = (ang_to_passos(angulo_m2)-(accel_m2/2)*t_m1**2)/t_m1
    else:
        vel_m2 = vel_init
        accel_m2 = accel
        accel_m1 = accel_m2

        delta = vel_m2 ** 2 - 4 * (accel_m2 / 2) * (-ang_to_passos(angulo_m2))
        t_m2 = (-vel_m2 + math.sqrt(delta)) / (2 * (accel_m2 / 2))
        vel_m1 = (ang_to_passos(angulo_m1) - (accel_m1 / 2) * t_m2 ** 2) / t_m2

    return vel_m1, vel_m2

# configure a porta serial
porta_serial = serial.Serial('COM4', 9600) # substitua COM4 pela porta serial em que o Arduino está conectado

while (True):
    angulo1 = int(input("Digite o angulo do motor 1: "))
    angulo2 = int(input("Digite o angulo do motor 1: "))

    accel = 250
    vel_init = 500

    vel_m1, vel_m2 = get_vel_motor(vel_init, accel, angulo1, angulo2)

    porta_serial.write(('{} {} {} {} {}\n'.format(vel_m1, vel_m2, accel, ang_to_passos(angulo1), ang_to_passos(angulo2))).encode())

    # espera o Arduino finalizar a tarefa
    while not porta_serial.in_waiting:
        pass
    print(porta_serial.readline().decode().strip())  # imprime a resposta do Arduino

# fecha a porta serial
porta_serial.close()