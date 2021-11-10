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
#include "semafori.h"

void Inizio_Lettura(int ds_sem){
	
}

void Fine_Lettura(){

}

void Inizio_Scrittura(int ds_sem){
	Wait_Sem(ds_sem, MUTEXS);
	buf->numscrittore = buf->numscrittore + 1;
	if(buf->nomescrittore == 1){
		Wait_Sem(ds_sem, SYNCH);
	}
	Signal_Sem(ds_sem, MUTEXS);
}

void Fine_Scrittura(){

}

void lettore(){

}

void scrittore(int ds_sem, Buffer *buf){
	Inizio_Lettura(ds_sem);

}
