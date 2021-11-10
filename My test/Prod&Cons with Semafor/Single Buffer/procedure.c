#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>
#include <semafori.h>
#include <procedure.h>

void produttore(int* p, int ds_sem){
	printf("Produttore in attesa!");
	Wait_Sem(ds_sem, SPAZIO DISPONIBILE);
	printf("Produttore si avvia!");
	*p = rand() % 100;
	printf("Il valore salvato è: %d\n",p);
	Signal_Sem(ds_sem, MESSAGGIO_DISPONIBILE);
}

void consumatore(int *p, int ds_sem){
	printf("Consumatore in attesa!");
	Wait_Sem(ds_sem, MESSAGGIO_DISPONIBILE);
	printf("Consumatore si avvia!");
	printf("Il messaggio letto è: %d\n",p);
	Signal_Sem(ds_sem, SPAZIO_DISPONIBILE);
}
