#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include "header.h"

int main(){
	int pid, status, queue1, queue2, queue3;

	queue1 = msgget(IPC_PRIVATE,IPC_CREAT | 0664);
	queue2 = msgget(IPC_PRIVATE,IPC_CREAT | 0664);
	queue3 = msgget(IPC_PRIVATE,IPC_CREAT | 0664);
	if(queue1==-1){perror("Errore Coda1!");exit(1);}
	if(queue2==-1){perror("Errore Coda2!");exit(1);}
	if(queue3==-1){perror("Errore Coda3!");exit(1);}

	for(int i=0; i<3; i++){
		pid = fork();
		if(pid==-1){perror("Errore Figlio!");exit(1);}
		if(pid==0){
			printf("Figlio generatore: %d\n",getpid());
			Generatore(queue1);
			exit(0);
		}
	}

	pid = fork();
	if(pid==-1){perror("Errore Figlio!");exit(1);}
	if(pid==0){
		printf("Figlio filtro: %d\n",getpid());
		Filtro(queue1,queue2);
		exit(0);
	}

	pid = fork();
	if(pid==-1){perror("Errore Figlio!");exit(1);}
	if(pid==0){
		printf("Figlio checksum: %d\n",getpid());
		Checksum(queue2,queue3);
		exit(0);
	}

	pid = fork();
	if(pid==-1){perror("Errore Figlio!");exit(1);}
	if(pid==0){
		printf("Figlio visualizzatore: %d\n",getpid());
		Visualizzatore(queue3);
		exit(0);
	}

	for(int i=0; i<6; i++){
		pid = wait(&status);
		printf("Processo figlio n: %d terminato!\n",pid);
	}

	msgctl(queue1,IPC_RMID,0);
	msgctl(queue2,IPC_RMID,0);
	msgctl(queue3,IPC_RMID,0);	

	return 0;
}
