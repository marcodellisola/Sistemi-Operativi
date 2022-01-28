#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>

#include "header.h"

#define NUM_THREADS_OPERANDI    2
#define NUM_THREADS_CALCOLO     3
int coda_risultati;

int main(){

        pthread_attr_t attr;
        pthread_t threads_operandi[NUM_THREADS_OPERANDI];
        pthread_t threads_calcolo[NUM_THREADS_CALCOLO];
    
        srand(time(NULL)*getpid());

        coda_risultati = /* TODO: Creazione coda risultati */msgget(IPC_PRIVATE,IPC_CREAT | 0664);
        printf("Coda coda_risultati create con desc: %d\n", coda_risultati);
    
        initServiceQueues();
            
        // Creazione di una istanza di struttura monitor per gli operandi
        // MonitorOperandi sarà usato dai thread generazione operandi
        MonitorOperandi * pc_op = /* TODO: inizializzare */malloc(sizeof(MonitorOperandi));
    
        /* TODO: inizializzazione mutex e condition */
	pthread_mutex_init(&(pc_op->mutex),NULL);
	pthread_cond_init(&(pc_op->ok_prod_cv),NULL);
	pthread_cond_init(&(pc_op->ok_cons_cv),NULL);
    
        /* TODO: inizializzazione delle variabili di stato */
	pc_op->num_occupati=0;
        pc_op->num_liberi=DIM_BUFFER;
        pc_op->elementi=0;
	pc_op->coda=0;
	pc_op->testa=0;

        /* TODO: impostazione dei thread come joinable */
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

        /* TODO: genero processo prelievo risultati */
	int pid = fork();
	if(pid==-1){perror("Errore processo figlio!");exit(1);}
	if(pid==0){
		printf("Inizio processo prelievo!\n");
		preleva_risultati(coda_risultati);
		exit(0);
	}
    
        /* TODO: genero thread generazione operandi */
	for(int i=0; i<2; i++){
		printf("Inizio thread generazione operandi!\n");
		pthread_create(&threads_operandi[i],&attr,genera_operandi,(void*)pc_op);
	}
    
        /* TODO: genero thread di calcolo */
	for(int i=0; i<3; i++){
		printf("Inizio thread calcolo!\n");
		pthread_create(&threads_calcolo[i],&attr,calcola,(void*)pc_op);
	}
    
        /* TODO: Join threads a wait processo*/
	for(int i=0; i<2; i++){
		pthread_join(threads_operandi[i],NULL);
		printf("Thread operando %d terminato!\n",i);
	}
	for(int i=0; i<3; i++){
		pthread_join(threads_calcolo[i],NULL);
		printf("Thread calcolo %d terminato!\n",i);
	}
	wait(NULL);
	printf("Processo figlio terminato!\n");
    
        /* TODO: deallocazione risorse */
	removeServiceQueues();
	pthread_mutex_destroy(&(pc_op->mutex));
	pthread_cond_destroy(&(pc_op->ok_prod_cv));
	pthread_cond_destroy(&(pc_op->ok_cons_cv));
	free(pc_op);
	msgctl(coda_risultati,IPC_RMID,0);
    
        pthread_exit(NULL);
}

