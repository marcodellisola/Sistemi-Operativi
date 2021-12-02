#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#include "procedure.h"

int main(){
	key_t k_shm;
	key_t k_sem;
	int ds_shm = shmget(k_shm, sizeof(struct prodcons), IPC_CREAT | 0664);
	if(ds_shm<0){perror("Errore SHM!");exit(1);}

	struct prodcons *p;
	p = (struct prodcons*) shmat(ds_sem, NULL, 0);
	if(p==(void*)-1){perror("Errore Attachment"); exit(1);}

	init_monitor(&(p->m),2);

	p->buffer_libero = 1;
	p->buffer_pieno = 0;

	int pid = fork();
	if(pid<0){perror("Errore Figlio!\n");exit(1);}
	if(pid==0){
		printf("Inizio consumatore!\n");
	       	int valore = consuma(p);
		printf("Valore consumato: %d\n",valore);
		exit(0);
	}

	pid=fork();
	if(pid<0){perror("Errore figlio!\n");exit(1);}
	if(pid==0){
		printf("Inizio Produttore!\n");
		int valore = rand%10;
		produci(p,valore);
		printf("Valore prodotto: %d\n",valore);
		exit(0);
	}

	for(int i=0;i<2;i++){
		wait(NULL);
	}
	remove_monitor(&(p->m));
	shmctl(ds_shm,IPC_RMID,0);

	return 0;
}

