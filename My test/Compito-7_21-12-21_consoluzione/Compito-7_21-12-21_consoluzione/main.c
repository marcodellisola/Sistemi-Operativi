#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "header.h"

#define NUM_CLIENT 3
#define NUM_SERVER 1

int main(){
	int i,status;
	int coda;
	pid_t pid;

	key_t key;
	key = ftok(".",'a');
    
	coda = msgget(key,IPC_CREAT | 0664);

	
	for( i = 0; i < NUM_CLIENT+NUM_SERVER; i++){
		
		char id_client = '1'+i;
		sleep(1);
		
		pid = fork();

		if(pid == 0){

			if(i == NUM_CLIENT+NUM_SERVER -1){
				printf("[SERVER %d] - Sono stato creato...\n",getpid());
				//TODO creare server con exec
				execl("./server","server");
				exit(-1);
			}else{
				printf("[CLIENT %d] - Sono stato creato con ID %c\n",getpid(),id_client);
				//TODO creare client con exec, passando anche il parametro &id_client
				execl("./client","client",id_client);
				exit(-1);
			}
		}
	}

	for(i = 0; i < NUM_CLIENT+NUM_SERVER;i++){

		wait(&status);

		if (WIFEXITED(status)) {
    			printf("[PADRE] - Figlio terminato con stato %d\n",status);
  		}
	}

	//TODO eliminare la coda
	msgctl(coda,IPC_RMID,0);
	printf("[PADRE] - Fine elaborazione...\n");


	return 0;	
}
