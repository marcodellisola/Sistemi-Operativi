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
	Buffer *buf;
	int ds_shm = shmget(k_shm, sizeof(Buffer), IPC_CREAT | 0664);
	if (ds_shm < 0){
		perror("Errore SHM!");
		exit(1);
	}
	int num_lettori = 6;
	int num_scrittori = 6;
	int num_processi = num_lettori + num_scrittori;
	buf = (Buffer*) shmat(ds_shm, NULL, 0);
	if ( buf == (void*)-1){
		perror("Errore Attachment!");
		exit(1);
	}
	buf->numlettori = 0;
	int ds_sem = semget(k_sem, 2, IPC_CREAT | 0664);
	semctl(ds_sem, MUTEXL, SETVAL, 1);
	semctl(ds_sem, SYNCH, SETVAL, 1);
	
	for(int i=0; i<num_processi; i++){
		int pid = fork();
		if(pid==-1){perror("Errore creazione processo figlio!"); exit(1);}
		if(pid == 0){
			if(i%2==0){
				printf("Sono il figlio scrittore: %d\n",getpid());
				scrittori(ds_sem,buf);
			}else{ 	
				printf("SOno il figlio lettore: %d\n",getpid());
				lettori(ds_sem,buf);}
			exit(0);
		}
	}
	
	for(int i=0; i<num_processi; i++){
		int pid=wait(&status);
		printf("Processo %d terminato!\n",pid);
	}

	shmctl(ds_shm, IPC_RMID, 0);
	semctl(ds_sem, 0, IPC_RMID);

	return 0;
}
		


