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

int main(){
	int status;
	key_t k_shm = IPC_PRIVATE;
	key_t k_sem = IPC_PRIVATE;
	int ds_shm = shmget(k_shm, sizeof(struct prodcons), IPC_CREAT | 0664);
	if(ds_shm<0){
		perror("Errore SHM!");
		exit(1);
	}

	Cliente* p[DIM_BUFFER] = (Cliente*) shmat(ds_shm, NULL, 0);
	if(p == (void*)-1){
		perror("Errore Attachment!");
		exit(1);
	}

	for(int i=0; i<DIM_BUFFER; i++){
		p[i].stato=BUFFER_VUOTO;
	}

	int ds_sem = semget(k_sem, 4, IPC_CREAT | 0664);
	if(ds_sem<0){
		perror("Errore SEM!");
		exit(1);
	}

	semctl(ds_sem, SPAZIO_DISPONIBILE, SETVAL, 80);
	semctl(ds_sem, MESSAGGIO_DISPONIBILE, SETVAL, 0);
	semctl(ds_sem, MUTEXS, SETVAL, 1);
	semctl(ds_sem, MUTEX, SETVAL,1);

	for(int i=0; i< NUM_PRODUTTORI; i++){
		int pid = fork();
		if(pid==-1){
			perror("Errore Figlio!");
			exit(1);
		}
		if(pid==0){
			int tempo = rand()%6;
			sleep(tempo);
			int pidcliente=getpid();
			prenotazione(ds_sem,p,pidcliente);
		}
	}
