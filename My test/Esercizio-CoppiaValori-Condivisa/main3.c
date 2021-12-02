#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "header.h"

int main(int argc, char *argv[]){
	key_t k_shm = 40;
        key_t k_sem = 41;
        Buffer *buf;
        int ds_shm = shmget(k_shm, sizeof(Buffer), IPC_CREAT | IPC_EXCL | 0664);
        if(ds_shm<0){
                ds_shm = shmget(k_shm, sizeof(Buffer), 0664);
                buf = shmat(ds_shm, NULL, 0);}
        int ds_sem = semget(ds_sem, 4, IPC_CREAT | IPC_EXCL | 0664);

        printf("Inizio lettore 2!");
        lettore(ds_sem,buf,2);
        return 0;
}

