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
	key_t k_shm = ftok(PATH_SHM,CHAR_SHM);
	key_t k_sem = ftok(PATH_SEM,CHAR_SEM);

	int ds_shm = shmget(k_shm,0,IPC_CREAT | 0664);
	if(ds_shm<0){
		perror("Errore SHM!");
		exit(1);
	}
	int ds_sem = semget(k_sem, 0, IPC_CREAT | 0664);
	if(ds_sem<0){
		perror("Errore SEM!");
		exit(1);
	}
	Buffer *buf;
	buf = (Buffer*) shmat(ds_shm, NULL, 0);
	if(buf==(void*)-1){
		perror("Errore Attachment!");
		exit(1);
	}
	Generatore(ds_sem, buf);
	sleep(1);

	return 0;
}
