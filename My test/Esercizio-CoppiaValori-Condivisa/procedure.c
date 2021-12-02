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

void Inizio_Lettura(int ds_sem, Buffer *buf){
	Wait_Sem(ds_sem,MUTEXL);
	buf->numlettori=buf->numlettori+1;
	if(buf->numlettori==1){
		Wait_Sem(ds_sem,SYNCH);
	}
	Signal_Sem(ds_sem,MUTEXL);
	Wait_Sem(ds_sem,MUTEX);
}

void Fine_Lettura(int ds_sem, Buffer *buf){
	Signal_Sem(ds_sem,MUTEX);
	Wait_Sem(ds_sem,MUTEXL);
	buf->numlettori=buf->numlettori-1;
	if(buf->numlettori==0){
		Signal_Sem(ds_sem,SYNCH);
	}
	Signal_Sem(ds_sem,MUTEXL);
}

void Inizio_Scrittura(int ds_sem, Buffer *buf){
	Wait_Sem(ds_sem, MUTEXS);
	buf->numscrittori=buf->numscrittori+1;
	if(buf->numscrittori==1){
		Wait_Sem(ds_sem, SYNCH);
	}
	Signal_Sem(ds_sem,MUTEXS);
	Wait_Sem(ds_sem,MUTEX);
}

void Fine_Scrittura(int ds_sem, Buffer *buf){
	Signal_Sem(ds_sem,MUTEX);
	Wait_Sem(ds_sem, MUTEXS);
	buf->numscrittori=buf->numscrittori-1;
	if(buf->numscrittori==0){
		Signal_Sem(ds_sem,SYNCH);
	}
	Signal_Sem(ds_sem,MUTEXS);
}

void lettore(int ds_sem, Buffer *buf,int val){
	Inizio_Lettura(ds_sem,buf);
	sleep(2);
	if(val==1){
		printf("Il val1 letto è: %d\n",buf->val1);
	}
	if(val==2){
		printf("Il val2 letto è: %d\n",buf->val2);
	}
	Fine_Lettura(ds_sem,buf);
}

void scrittore(int ds_sem, Buffer *buf){
	Inizio_Scrittura(ds_sem, buf);
	int x;
	while(x>9){
		x = rand();
		if(x<10){	
			buf->val1 = x;
		}	
	}
	while(x>9){
		x = rand();
		if(x<10){
			buf->val2 = x;
		}
	}
	sleep(1);
	printf("Il val1 scritto è: %d\n", buf->val1);
	printf("Il val2 scritto è: %d\n", buf->val2);
	Fine_Scrittura(ds_sem, buf);
}
