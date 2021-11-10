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
void Inizio_Lettura(int ds_sem, Buffer *buf){
        Wait_Sem(ds_sem, MUTEXL);
        buf->numlettori = buf->numlettori + 1;
        if(buf->numlettori == 1){
                Wait_Sem(ds_sem, SYNCH);
        }
        Signal_Sem(ds_sem, MUTEXL);
}

void Fine_Lettura(int ds_sem, Buffer *buf){
        Wait_Sem(ds_sem, MUTEXL);
        buf->numlettori = buf->numlettori - 1;
        if(buf->numlettori == 0){
                Signal_Sem(ds_sem, SYNCH);
        }
        Signal_Sem(ds_sem, MUTEXL);
}

void Inizio_Scrittura(int ds_sem){
        Wait_Sem(ds_sem, SYNCH);
}

void Fine_Scrittura(int ds_sem){
        Signal_Sem(ds_sem, SYNCH);
}

void lettori(int ds_sem, Buffer *buf){
	Inizio_Lettura(ds_sem, buf);
	sleep(1);
	printf("Il dato letto è: %ld\n", buf->messaggio);
	Fine_Lettura(ds_sem, buf);
}

void scrittori(int ds_sem, Buffer *buf){
	Inizio_Scrittura(ds_sem);
	struct timeval t1;
	struct timezone t2;
	gettimeofday(&t1,&t2);
	msg val = t1.tv_usec;
	buf->messaggio = val;
	sleep(1);
	printf("Messaggio scritto: %ld\n",buf->messaggio);
	Fine_Scrittura(ds_sem);	
}
