#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "header.h"
#include "semafori.h"

void Inizio_Lettura(int ds_sem, Buffer *buf){
	Wait_Sem(ds_sem, MUTEXL);
	buf->numlettori = buf->numlettori + 1;
	if(buf->numlettori == 1){
		Wait_Sem(ds_sem, SYNCH);
	}
	Signal_Sem(ds_sem, MUTEXL);
	Wait_Sem(ds_sem, MUTEX);
}

void Fine_Lettura(int ds_sem, Buffer *buf){
	Signal_Sem(ds_sem, MUTEX);
	Wait_Sem(ds_sem, MUTEXL);
	buf->numlettori = buf->numlettori - 1;
	if(buf->numlettori == 0){
		Signal_Sem(ds_sem, SYNCH);
	}
	Signal_Sem(ds_sem, MUTEXL);
}

void Inizio_Scrittura(int ds_sem, Buffer *buf){
	Wait_Sem(ds_sem, MUTEXS);
	buf->numscrittori = buf->numscrittori + 1;
	if(buf->numscrittori == 1){
		Wait_Sem(ds_sem, SYNCH);
	}
	Signal_Sem(ds_sem, MUTEXS);
	Wait_Sem(ds_sem, MUTEX);
}

void Fine_Scrittura(int ds_sem, Buffer *buf){
	Signal_Sem(ds_sem, MUTEX);
	Wait_Sem(ds_sem, MUTEXS);
	buf->numscrittori = buf->numscrittori - 1;
	if(buf->numscrittori == 0){
		Signal_Sem(ds_sem, SYNCH);
	}
	Signal_Sem(ds_sem, MUTEXS);
}

void lettore(int ds_sem, Buffer *buf){
	Inizio_Lettura(ds_sem, buf);
	sleep(2);
	printf("Il valore letto è: %ld\n", buf->messaggio);
	Fine_Lettura(ds_sem, buf);
}

void scrittore(int ds_sem, Buffer *buf){
	Inizio_Lettura(ds_sem, buf);
	struct timeval t1;
	struct timezone t2;
	gettimeofday(&t1, &t2);
	buf->messaggio = t1.tv_usec;
	sleep(2);
	printf("Il valore scritto è: %ld\n", buf->messaggio);
	Fine_Scrittura(ds_sem, buf);
}
