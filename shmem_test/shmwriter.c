#include <stdio.h>

#include <string.h>

#include <sys/types.h>

#include <sys/ipc.h>

#include <sys/shm.h>

#include <signal.h>

#define SIZE 1024

 

void signalHandler(int signo);

int shmid;

 

main(){
    void *shmaddr;
    if((shmid=shmget((key_t)777, SIZE, IPC_CREAT|0666)) == -1) {
       perror("shmid failed");
       exit(1);
    }
    if((shmaddr=shmat(shmid, (void *)0, 0)) == (void *)-1) {
       perror("shmat failed");
       exit(1);
    }
    strcpy((char *)shmaddr, "Security Codename bitnang");
    if(shmdt(shmaddr) == -1) {
       perror("shmdt failed");
       exit(1);
    }
    signal(SIGINT, signalHandler);
    pause();
}

 

void signalHandler(int signo){
    if(shmctl(shmid, IPC_RMID, 0) == -1) {
       perror("shmctl failed");
       exit(1);
    }
    exit(0);

}
//[출처] C언어_시스템프로그래밍 : 공유 메모리를 이용한 통신|작성자 bitnang