#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

#include <sys/sem.h>
#include "procedure.h"

int main(){
	key_t k_shm = IPC_PRIVATE;
	key_t k_sem = IPC_PRIVATE;
	int ds_shm = shmget(k_shm, sizeof(struct prodcons), IPC_CREAT | 0664);
	int ds_sem = semget(k_sem, 4, IPC_CREAT | 0664);
	if(ds_shm<0){perror("Errore SHM!"); exit(1);}
	if(ds_sem<0){perror("Errore SEM!"); exit(1);}

	struct prodcons* p;
	p = shmat(ds_shm, NULL, 0);
	if(p==(void*)-1){perror("Errore attachment!"); exit(1);}

	p->testa=0;
	p->coda=0;

	semctl(ds_sem, SPAZIO_DISPONIBILE, SETVAL, DIM_BUFFER);
	semctl(ds_sem, MESSAGGIO_DISPONIBILE, SETVAL, 0);
	semctl(ds_sem, MUTEX_P, SETVAL, 1);
	semctl(ds_sem, MUTEX_C, SETVAL, 1);

	for(int i=0; i<NUM_CONSUMATORI; i++){
		int pid = fork();
		if(pid==-1){perror("Errore processo figlio!"); exit(1);}
		if(pid==0){
			i=NUM_CONSUMATORI;
			printf("Figlio consumatore!");
			srand(getpid()*time(NULL));
			consumatore(p, ds_sem);
			exit(0);
		}
	}

	for(int i=0; i<NUM_PRODUTTORI; i++){
		int pid = fork();
		if(pid==-1){perror("Errore processo figlio!"); exit(1);}
		if(pid==0){
			i=NUM_CONSUMATORI;
			printf("Figlio produttore!");
			srand(getpid()*time(NULL));
			produttore(p, ds_sem);
			exit(0);
		}
	}

	for(int i=0; i<NUM_CONSUMATORI; i++){
		wait(NULL);
		printf("Figlio Consumatore terminato!");
	}
	for(int i=0; i<NUM_PRODUTTORI; i++){
		wait(NULL);
		printf("Figlio Produttore terminato!");
	}

	shmctl(ds_shm, IPC_RMID, NULL);
	semctl(ds_sem, 0, IPC_RMID);	

	return 0;
}
