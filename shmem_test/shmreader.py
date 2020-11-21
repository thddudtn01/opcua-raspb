import sysv_ipc
 
 
class CShmReader :
 
    def __init__( self ) :
 
        pass
 
    def doReadShm( self , key ) :
 
        memory = sysv_ipc.SharedMemory( key )
 
        memory_value = memory.read()
        print ( memory_value )
 
if __name__ == '__main__':
 
    s = CShmReader()
    s.doReadShm( 777 )


#ref: https://speedr00t.tistory.com/484 [Black Falcon]