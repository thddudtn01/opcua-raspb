import sysv_ipc
import RPi.GPIO as GPIO
import time

class CShmReader :
 
    def __init__( self ) :
        pass
 
    def doReadShm( self , key ) :
        memory = sysv_ipc.SharedMemory( key )
        memory_value = memory.read()
        print ( memory_value )
        
if __name__ == '__main__':


    memory = sysv_ipc.SharedMemory( 777, flags=01000, size=10 ,mode=0600)
    memory.write("1")
    s = CShmReader()
    s.doReadShm( 777 )

    GPIO.setmode(GPIO.BCM)

    print "Setup LED pins as outputs"

    GPIO.setup(23, GPIO.OUT)
    GPIO.setup(24, GPIO.OUT)
    GPIO.setup(25, GPIO.OUT)

    try:
        while True:
            light = s.doReadShm( 777 )
            if(light == '1'):
                GPIO.output(23, True)
                GPIO.output(24, False)
                GPIO.output(25, False)
            if(light == '2'):
                GPIO.output(23, False)
                GPIO.output(24, True)
                GPIO.output(25, False)
            if(light == '3'):
                GPIO.output(23, False)
                GPIO.output(24, False)
                GPIO.output(25, True)
                time.sleep(0.1)

    except KeyboardInterrupt:
        GPIO.cleanup()

