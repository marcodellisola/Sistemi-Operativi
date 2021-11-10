#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
	int pid,stato;
	pid = fork();	
	if(pid == -1){
		perror("Il processo figlio non è stato creato!");
		exit(1);
	}

	if(pid == 0){
		execl("/bin/ls", "ls", "-l", NULL);
		if(execl("/bin/ls", "ls", "-l", NULL)<0){
			perror("Exec fallita");
			exit(1);
		}
		exit(0);
	}

	if (pid > 0){
		wait(&stato);
		printf("Siamo nel processo padre, esecuzione avvenuta con successo!");
	}

}
