#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include "header.h"

int main(){
	int pid,status;
	key_t k_shm = IPC_PRIVATE;
	int ds_shm = shmget(k_shm, sizeof(meteo), IPC_CREAT | 0664);
	if(ds_shm<0){perror("Errore SHM!");exit(1);}
	meteo* p;
	p = (meteo*) shmat(ds_shm,NULL,0);
	if(p==(void*)-1){perror("Errore Attachment!");exit(1);}
	init_monitor(&(p->m), 1);

	p->buffer_lettori=0;

	//Meteo
	pid = fork();
	if(pid==-1){perror("Errore Figlio!");exit(1);}
	if(pid==0){
		printf("Produzione previsione!\n");
		Scrittore(p);
		exit(0);
	}

	for(int i=0; i<NUM_UTENTI; i++){
		pid = fork();
		if(pid==-1){perror("Errore Figlio!");exit(1);}
		if(pid==0){
			printf("Utente n: %d\n",getpid());
			Lettore(p);
			exit(0);
		}
	}

	for(int i=0; i<NUM_UTENTI+1;i++){
		pid = wait(&status);
		printf("Processo n: %d terminato!\n",pid);
	}

	shmctl(ds_shm,IPC_RMID,0);
	return 0;
}
