#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include "header.h"

int main(int argc, char *argv[]){
	int pid,status, queue1,queue2;
	char firstchar = *argv[1];
	char secondchar = *argv[2];
	key_t k_queue1 = ftok(FTOK_PATH, firstchar);
	key_t k_queue2 = ftok(FTOK_PATH, secondchar);
	queue1 = msgget(k_queue1, IPC_CREAT | 0664);
	queue2 = msgget(k_queue2, IPC_CREAT | 0664);

	pid = fork();
	if(pid==-1){perror("Errore figlio!");exit(1);}
	if(pid==0){
		printf("Processo di scrittura: %d\n",getpid());
		Scrittore(queue1);
		exit(0);
		}

	pid = fork();
        if(pid==-1){perror("Errore figlio!");exit(1);}
        if(pid==0){
                printf("Processo di lettura: %d\n",getpid());
		Lettore(queue2);
	}

	for(int i=0;i<2;i++){
		pid = wait(&status);
		printf("Processo %d terminato!\n",pid);
	}

	msgctl(queue1,IPC_RMID,0);
	msgctl(queue2,IPC_RMID,0);
	return 0;
}
