#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
	int pid, status;
	pid = fork();
	if(pid == 0){
		execl(argv[2],argv[1],NULL);
		if(execl(argv[2],argv[1],NULL)<0){
			perror("Esecuzione programma fallita!");
			exit(1);
		}
		exit(0);
	} else if(pid == -1){
		perror("Processo figlio non creato!");
		exit(1);
		}
	if(pid > 0){
		wait(&status);
		printf("Il processo figlio è terminato! Siamo nel processo genitore.");
		exit(0);
	}






}
