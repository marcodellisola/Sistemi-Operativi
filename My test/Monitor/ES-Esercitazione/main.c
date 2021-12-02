#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>

#include "prodcons.h"

int main(){
	int pid;
	key_t k_shm = IPC_PRIVATE;
    	int ds_shm = shmget(k_shm,sizeof(ProdCons),IPC_CREAT | 0664);
    	if (ds_shm < 0)
    	{
        	perror("Errore creazione shared memory");
        	exit(1);
    	}

    	ProdCons *p = shmat(ds_shm,NULL,0);
    	if (p == (void *)-1)
    	{
        	perror("Errore attach shared memory");
        	exit(1);
    	}
	p->testa=0;
	p->coda=0;
	p->totale_elementi=0;

    	inizializza(p);

    	for(int i=0; i<5; i++){
		pid = fork();
		if(pid==-1){perror("Errore figlio produttore!");exit(1);}
		if(pid==0){
			for(int i=0; i<9; i++){
				printf("Inizio Produzione!\n");
				int val = rand();
				produci(p, val);
				exit(0);
			}
		}
	}

	for(int i=0; i<5; i++){
		pid = fork();
		if(pid==-1){perror("Errore figlio consumatore!");exit(1);}
		if(pid==0){
			printf("Inizio Consumazione!\n");
			consuma(p);
			exit(0);
		}
	}

    printf("[%d] Processo padre in attesa...\n", getpid());

    for (int i = 0; i < 10; i++)
    {
        wait(NULL);
    }

    printf("[%d] Terminazione\n", getpid());

    rimuovi(p);
    shmctl(ds_shm,IPC_RMID,0);

    return 0;
}
