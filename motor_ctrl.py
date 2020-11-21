from gpiozero import Motor
import time

motor = Motor(forward=20, backward=21)

while True:
    motor.forward(speed=1)
    time.sleep(5)

    motor.backward(speed=1)
    time.sleep(5)