import sysv_ipc
class CShmReader :
 
    def __init__( self ) :
        pass
 
    def doReadShm( self , key ) :
        memory = sysv_ipc.SharedMemory( key )
        memory_value = memory.read()
        print ( memory_value )
        
if __name__ == '__main__':


    memory = sysv_ipc.SharedMemory( 1234, flags=01000, size=10 ,mode=0600)
    memory.write(7)
    s = CShmReader()
    s.doReadShm( 777 )

