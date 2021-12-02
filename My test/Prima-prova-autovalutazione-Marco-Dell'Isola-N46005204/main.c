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

int main() {
	int status;
	key_t k_shm = ftok(PATH_SHM,CHAR_SHM);
	key_t k_sem = ftok(PATH_SEM,CHAR_SEM);

	int ds_shm = shmget(k_shm, sizeof(Buffer), IPC_CREAT | IPC_EXCL | 0664);
	if(ds_shm<0){
		perror("Errore SHM!");
		exit(1);
	}
	
	Buffer *buf;
	buf = (Buffer*) shmat(ds_shm, NULL, 0);
	if(buf == (void*)-1){
		perror("Errore Attachment!");
		exit(1);
	}
	buf->numlettori = 0;
	buf->N=(rand()%6)+10;
	for(int i=0; i<buf->N; i++){
                char lettera = 'a' + (rand()%26);
                buf->str[i]=lettera;
        }

	int ds_sem = semget(k_sem, 2, IPC_CREAT | IPC_EXCL | 0664);
	if(ds_sem<0){
		perror("Errore SEM!");
		exit(1);
	}
	semctl(ds_sem, MUTEXL, SETVAL, 1);
	semctl(ds_sem, SYNCH, SETVAL, 1);

	int pid = fork();
	if(pid==-1){
		perror("Errore processo figlio!\n");
		exit(1);
	}
	if(pid==0){
		printf("Inizio Processo Generatore!\n");
		execl("./generatore", "generatore", NULL);
		exit(0);
	}

	pid = fork();
	if(pid==-1){
		perror("Errore processo figlio!\n");
		exit(1);
	}
	if(pid==0){
		printf("Inizio Processo elaboratore!\n");
		execl("./elaboratore", "elaboratore", NULL);
		exit(0);
	}
	for(int i=0; i<2;i++){
		pid = fork();
		if(pid==-1){
			perror("Errore processo figlio!\n");
			exit(1);
		}
		if(pid==0){
			printf("Inizio processo analizzatore!\n");
			execl("./analizzatore", "analizzatore", NULL);
			exit(0);
		}
	}

	for(int i=0; i<3; i++){
		pid = wait(&status);
		printf("Processo figlio %d terminato!\n", pid);
	}
	
	shmctl(ds_sem, IPC_RMID, NULL);
	semctl(ds_sem, 0, IPC_RMID);

	return 0;
}

