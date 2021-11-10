#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> //Per 
#include <stdlib.h> //Per fare l'exit

int main(){
	int pid;
	pid = fork();
	
	if(pid == -1){
		perror("Il processo figlio non è stato creato!\n");
		exit(1);
	}

	if(pid == 0){
		printf("Il processo figlio è stato creato, e il pid è: %d\n",getpid());
		exit(0);
	}

	if(pid > 0){
		printf("Questo è il processo padre. Il pid del processo figlio è %d, il pid del processo padre è: %d\n", pid,getpid());
		exit(0);
	}

}
