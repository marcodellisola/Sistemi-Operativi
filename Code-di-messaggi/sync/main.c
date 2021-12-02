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
	int pid,status,queue;
	queue = msgget(IPC_PRIVATE,IPC_CREAT | 0664);
	int NUM_PROCESSI = NUM_PRODUTTORI + NUM_CONSUMATORI;
	char msg[30];

	pid = fork();
	if(pid==-1){perror("Errore Figlio!");exit(1);}
	if(pid==0){
		printf("Figlio produttore con pid: %d\n",getpid());
		for(int i=0;i<NUM_PRODUTTORI;i++){
			sprintf(msg,"Stringa %d",i);
			Produttore(queue,msg);
		}
		exit(0);
	}

	pid = fork();
	if(pid==-1){perror("Errore Figlio!");exit(1);}
	if(pid==0){
		printf("Figlio consumatore con pid: %d",getpid());
		for(int i=0;i<NUM_CONSUMATORI;i++){
			Consumatore(queue);
		}
		exit(0);
	}

	for(int i=0;i<2;i++){
		pid = wait(&status);
		printf("Figlio n°: %d terminato!\n",pid);
	}

	msgctl(queue,IPC_RMID,0);
	return 0;
}
