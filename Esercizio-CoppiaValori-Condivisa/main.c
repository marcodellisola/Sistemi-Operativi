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

int main(int agrc, char *argv[]){
	int status;

	int pid = fork();
	if(pid==-1){perror("Errore Processo Scrittore!");exit(1);}
	if(pid==0){
		printf("Processo Scrittore: %d\n", getpid());
		execl("/root/Desktop/Esercizio-CoppiaValori-Condivisa/scritt","scritt" ,NULL);
		exit(0);
	}
	//Processo lettore
	pid = fork();
	if(pid==-1){perror("Errore Processo Lettore!");exit(1);}
	if(pid==0){
		printf("Processo Lettore: %d\n", getpid());
		execl("/root/Desktop/Esercizio-CoppiaValori-Condivisa/lett1","lett1",NULL);
		exit(0);
	}
        pid = fork();
        if(pid==-1){perror("Errore Processo Lettore!");exit(1);}
        if(pid==0){
                printf("Processo Lettore: %d\n", getpid());
                execl("/root/Desktop/Esercizio-CoppiaValori-Condivisa/lett2","lett2",NULL);
                exit(0);
        }   

	for(int i=0; i<3; i++){
		pid = wait(&status);
		printf("Processo n° %d terminato!\n", pid);
	}

	return 0;
}
