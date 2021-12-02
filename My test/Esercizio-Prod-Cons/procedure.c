#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "header.h"

void prenotazione(int ds_sem, Cliente *p,int pidcliente){
	int posti = rand()%5;
	if(posti<=4 && posti<SPAZIO_DISPONIBILE){
		Wait_Sem(ds_sem, MUTEXS);
		Wait_Sem(ds_sem, MUTEX);
		Wait_Sem(ds_sem, SPAZIO_DISPONIBILE);
		semctl(ds_sem, SPAZIO_DISPONIBILE, SETVAL, SPAZIO_DISPONIBILE-posti);
		int i=0;
		while(i <= DIM_BUFFER && p->stato[indice] != BUFFER_VUOTO) {
        i++;
    }
		int j=i;
		for(int k=0;k<posti;k++){
			p[i].stato=BUFFER_INUSO;
			i++;
		}
		Signal_Sem(ds_sem, MUTEXS);
		for(int k=0;k<posti;k++){
                        p[j].stato=BUFFER_PIENO;
			p[j].id_cliente=pidcliente;
                        j++;
                }
		Signal_Sem(ds_sem, MESSAGGIO_DISPONIBILE);
		Signal_Sem(ds_sem,MUTEX);
	}else{ printf("Posti non disponibili!");
			exit(0);}
}

void lettura(int ds_sem, Cliente *p){
	Wait_Sem(ds_sem, MESSAGGIO_DISPONIBILE);
	Wait_Sem(ds_sem, MUTEX);
	int i=0;
	while(p[i].stato!=BUFFER_VUOTO){
		printf("Il Cliente %d occupa il posto %d, il posto risulta %d.", p[i].id_cliente,i,p[i].stato);
		i++;
	}
	Signal_Sem(ds_sem, MUTEX);
}
