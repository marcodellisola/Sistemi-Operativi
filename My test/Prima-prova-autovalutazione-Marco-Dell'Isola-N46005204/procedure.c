#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>

#include "header.h"
#include "semafori.h"

void InizioLettura(int sem, Buffer* buf){
    
    Wait_Sem(sem, MUTEXL);
    buf->numlettori = buf->numlettori + 1;
    
    if (buf->numlettori == 1)
        Wait_Sem(sem, SYNCH);
    
    Signal_Sem(sem, MUTEXL);
}

void FineLettura(int sem, Buffer* buf){
    
    Wait_Sem(sem, MUTEXL);
    buf->numlettori = buf->numlettori - 1;
    
    if (buf->numlettori == 0) 
        Signal_Sem(sem, SYNCH);
    
    Signal_Sem(sem, MUTEXL);
}


void InizioScrittura(int sem){
	Wait_Sem(sem, SYNCH);
}


void FineScrittura (int sem){
	Signal_Sem(sem, SYNCH);
}

void Elaboratore(int sem, Buffer *buf){
	while(1){
		sleep(1);
		InizioLettura(sem, buf);
		printf("La stringa in maiuscolo è: ");
		for(int i=0; i<buf->N;i++){
			buf->str[i]=buf->str[i]-32;
			printf("%c",buf->str[i]);
		}
		printf("\n");
		FineLettura(sem, buf);
	}
}

void Generatore(int sem, Buffer* buf) {
	while(1){
		sleep(3);
        	InizioScrittura(sem);
        	for(int i=0; i<buf->N; i++){
                	char lettera = 'a' + (rand()%26);
                	buf->str[i]=lettera;
        	}
        	FineScrittura(sem);
	}
}

void Analizzatore (int sem, Buffer* buf) {
	while(1){
		sleep(2);
        	InizioLettura(sem, buf);
		char strc[NMAX];
		printf("La stringa invertita è: ");
        	for(int i=0; i<buf->N; i++){
			strc[i]=buf->str[buf->N-i-1];
		}
		for(int i=0; i<buf->N; i++){
			buf->str[i]=strc[i];
			printf("%c",buf->str[i]);
		}
		printf("\n");
        	FineLettura(sem, buf);
	}
}
