#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "header.h"


void generatore_produttore(struct ProdConsGen *pc){
        
    /* TODO: utilizzare il costrutto monitor per la produzione del messaggio */
	enter_monitor(&(pc->m));
	if(pc->num_liberi<0){
		wait_condition(&(pc->m),VARCOND_GEN_PRODUTTORI);
	}
	message m;
	int i;
	for(i=0; i<STRING_MAX_DIM; i++){
		m.stringa[i] ='a' + (rand() %26);
	}
	m.stringa[i++]='\0';
	m.arr[0]=rand()%11;
	m.arr[1]=rand()%11;
	m.var=0;
	m.type=MSG_TYPE;
	pc->num_occupati++;
	pc->num_liberi--;
	pc->buffer[pc->testa]=m;
	pc->testa=(pc->testa +1) % DIM_QUEUE;
	pc->elementi++;

    
    printf("[generatore_produttore] Messaggio generato!\n");
    printf("[generatore_produttore] ...............msg.stringa: %s\n", /* TODO */m.stringa);
    printf("[generatore_produttore] ...............msg.array[0]: %d\n", /* TODO */m.arr[0]);
    printf("[generatore_produttore] ...............msg.array[1]: %d\n", /* TODO */m.arr[1]);
    printf("[generatore_produttore] ...............msg.var: %d\n", /* TODO */m.var);
 
    /* TODO: ... */
	signal_condition(&(pc->m),VARCOND_GEN_CONSUMATORI);
	leave_monitor(&(pc->m));
}

void generatore_consumatore(struct ProdConsGen *pc, int ds_queue_gen_filter){

    /* TODO: utilizzare il costrutto monitor per la consumazione del messaggio e l'invio verso il processo filter */
	enter_monitor(&(pc->m));
	if(pc->num_occupati<0){
		wait_condition(&(pc->m),VARCOND_GEN_CONSUMATORI);
	}
	message m;
	m=pc->buffer[pc->coda];
	pc->coda=(pc->coda + 1) % DIM_QUEUE;
	pc->elementi--;
	pc->num_liberi++;
	pc->num_occupati--;
    
    printf("[generatore_consumatore] Messaggio CONSUMATO!\n");
    printf("[generatore_consumatore] ...............msg.stringa: %s\n", pc->buffer[pc->coda].stringa);
    printf("[generatore_consumatore] ...............msg.array[0]: %d\n", pc->buffer[pc->coda].arr[0]);
    printf("[generatore_consumatore] ...............msg.array[1]: %d\n", pc->buffer[pc->coda].arr[1]);
    printf("[generatore_consumatore] ...............msg.var: %d\n", pc->buffer[pc->coda].var);
    
    int ret = /* TODO: invio del messaggio consumato al processo filter */msgsnd(ds_queue_gen_filter,&m,sizeof(message)-sizeof(long),0);
    
    if (ret<0){
            perror("msgsnd del messaggio on ds_queue_gen_filter FALLITA!");
            exit(-1);
    }
    printf("[generatore_consumatore] Messaggio INVIATO!\n");
    
    /* TODO: ... */
    signal_condition(&(pc->m),VARCOND_GEN_PRODUTTORI);
    leave_monitor(&(pc->m));
    exit(0);
}

