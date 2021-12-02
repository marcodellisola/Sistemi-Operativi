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
	char m[30];
	queue = msgget(IPC_PRIVATE, IPC_CREAT | 0664);
	int NUM_PROCESSI = NUM_PRODUTTORI + NUM_CONSUMATORI;
	pid = fork();
	if(pid == -1) {perror("Errore Figlio!");exit(1);};
	if(pid == 0){
		printf("Sono il produttore. Il mio pid è: %d\n",getpid());
		for(int i=0; i<NUM_PRODUTTORI; i++){
			sprintf(m,"Stringa %d",i);
			Produttore(queue,m);
		}
		exit(0);
	}
	
	pid = fork();
	if(pid == -1){perror("Errore Figlio!");exit(1);}
	if(pid == 0){
		printf("Sono il figlio consumatore. Il mio pid è: %d\n",getpid());
		for(int i=0; i<NUM_CONSUMATORI; i++){
			Consumatore(queue);
		}
		exit(0);
	}

	for(int i=0; i<NUM_PROCESSI; i++){
		int pid = wait(&status);
		printf("Processo figlio con pid: %d terminato!",pid);
	}

	msgctl(queue,IPC_RMID,0);

	return 0;
}
