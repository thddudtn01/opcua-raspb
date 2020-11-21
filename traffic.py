import sysv_ipc
import RPi.GPIO as GPIO
import time

memory = sysv_ipc.SharedMemory( 777, flags=01000, size=10, mode=0600)
memory.write("000")

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
        else:
            GPIO.output(23, False)
        if(light[1] == '1'):
            GPIO.output(24, True)
        else:
            GPIO.output(24, False)
        if(light[2] == '1'):
            GPIO.output(25, True)
        else:
            GPIO.output(25, False)
        time.sleep(0.1)

except KeyboardInterrupt:
    GPIO.cleanup()

