#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <sys/sem.h>
#include "procedure.h"
#include <time.h>

void produttore(struct prodcons *p, int ds_sem){
	printf("Produttore in attesa!");
	Wait_Sem(ds_sem, SPAZIO_DISPONIBILE);
	Wait_Sem(ds_sem, MUTEX_P);
	printf("Produttore avviato!");

	p->buffer[p->testa] = rand()%100;

	printf("Valore inserito: %d\n",p->buffer[p->testa]);
	p->testa = (p->testa+1) % DIM_BUFFER;

	Signal_Sem(ds_sem, MESSAGGIO_DISPONIBILE);
	Signal_Sem(ds_sem, MUTEX_P);
}

void consumatore(struct prodcons *p, int ds_sem){
	printf("Consumatore in attesa!");
	Wait_Sem(ds_sem, MESSAGGIO_DISPONIBILE);
	Wait_Sem(ds_sem, MUTEX_C);
	printf("Consumatore avviato!");

	printf("Il valore letto è: %d\n",p->buffer[p->coda]);
	p->coda = (p->coda+1)%DIM_BUFFER;

	Signal_Sem(ds_sem, SPAZIO_DISPONIBILE);
	Signal_Sem(ds_sem, MUTEX_C);
}


