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
	int ds_shm = shmget(k_shm, sizeof(Buffer), IPC_CREAT | 0664);
	Buffer *buf;
	if(ds_shm<0){perror("Errore SHM!"); exit(1);}
	buf = (Buffer*) shmat(ds_shm, NULL, 0);
	if (buf == (void*)-1){perror("Errore SHM attachment!"); exit(1);}
	
	buf->numlettori = 0;
	buf->numscrittori = 0;
	int num_scrittori = 6;
	int num_lettori = 6;
	int num_processi = num_scrittori + num_lettori;

	int ds_sem = semget(k_sem, 4, IPC_CREAT | 0664);
	if(ds_sem<0){perror("Errore SEM!"); exit(1);}

	semctl(ds_sem, SYNCH, SETVAL, 1);
	semctl(ds_sem, MUTEXL, SETVAL, 1);
	semctl(ds_sem, MUTEXS, SETVAL, 1);
	semctl(ds_sem, MUTEX, SETVAL, 1);

	for(int i=0; i<num_processi; i++){
		int pid = fork();
		if(pid==-1){perror("Errore creazione processo figlio!"); exit(1);}
		if(pid==0){
			if((i%2)==0){
				printf("Processo scrittore n°: %d\n",getpid());
				scrittore(ds_sem, buf);
				exit(0);
			}else{
				printf("Processo lettore n°: %d\n", getpid());
				lettore(ds_sem, buf);
				exit(0);
			}
		}
	}

	for(int i=0; i<num_processi; i++){
		int pid=wait(&status);
		printf("Processo %d terminato!\n", pid);
	}

	shmctl(ds_shm, IPC_RMID, NULL);
	semctl(ds_sem, 0, IPC_RMID);

	return 0;
}
