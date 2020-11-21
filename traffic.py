import sysv_ipc
import RPi.GPIO as GPIO
import time

memory = sysv_ipc.SharedMemory( 777, flags=01000, size=10 ,mode=0600)
memory.write("3")

GPIO.setmode(GPIO.BCM)

print "Setup LED pins as outputs"

GPIO.setup(23, GPIO.OUT)
GPIO.setup(24, GPIO.OUT)
GPIO.setup(25, GPIO.OUT)
try:
    while True:
        light = memory.read()
        if(light[0] == "1"):
            GPIO.output(23, True)
            GPIO.output(24, False)
            GPIO.output(25, False)
        if(light[0] == '2'):
            GPIO.output(23, False)
            GPIO.output(24, True)
            GPIO.output(25, False)
        if(light[0] == '3'):
            GPIO.output(23, False)
            GPIO.output(24, False)
            GPIO.output(25, True)
        time.sleep(0.1)

except KeyboardInterrupt:
    GPIO.cleanup()

