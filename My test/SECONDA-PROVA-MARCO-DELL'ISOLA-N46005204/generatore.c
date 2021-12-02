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
    	enter_monitor(&(pc->m));
    	printf("Ingresso generatore_produttore!\n");
    	while((pc->totale_elementi)>=4){
			printf("Processo generatore_produttore in attesa!\n");
		      	wait_condition(&(pc->m),VARCOND_GEN_PRODUTTORI);
			printf("Processo generatore_produttore riprende!\n");
	}
	leave_monitor(&(pc->m));

	message m;
	m.tipo=MSG_TYPE;
	int l=rand()%10+1;
        for(int i=0; i<l; i++){
        	m.string[i]='a'+(rand()%26);
        }
        m.string[l]='\0';
       	for(int i=0; i<2; i++){
        	m.ar[i]=rand()%10;
        }
        m.var=0;

    printf("[generatore_produttore] Messaggio generato!\n");
    printf("[generatore_produttore] ...............msg.stringa: %s\n", m.string);
    printf("[generatore_produttore] ...............msg.array[0]: %d\n", m.ar[0]);
    printf("[generatore_produttore] ...............msg.array[1]: %d\n",	m.ar[1]);
    printf("[generatore_produttore] ...............msg.var: %d\n", m.var);
 	
    enter_monitor(&(pc->m));
    pc->msg[pc->testa]=m;
    pc->testa=(pc->testa+1)%DIM_QUEUE;
    pc->totale_elementi++;
    signal_condition(&(pc->m),VARCOND_GEN_CONSUMATORI);
    leave_monitor(&(pc->m));
}

void generatore_consumatore(struct ProdConsGen *pc, int ds_queue_gen_filter){

    /* TODO: utilizzare il costrutto monitor per la consumazione del messaggio e l'invio verso il processo filter */
    enter_monitor(&(pc->m));
    printf("Ingresso monitor consumatore!\n");
    while((pc->totale_elementi)<1){
	    printf("Processo generatore_consumatore in attesa!\n");
	    wait_condition(&(pc->m),VARCOND_GEN_CONSUMATORI);
	    printf("Processo generatore_produttore riprende!\n");
    }
    leave_monitor(&(pc->m));
	
    printf("[generatore_consumatore] Messaggio CONSUMATO!\n");
    printf("[generatore_consumatore] ...............msg.stringa: %s\n", pc->msg[pc->coda].string);
    printf("[generatore_consumatore] ...............msg.array[0]: %d\n", pc->msg[pc->coda].ar[0]);
    printf("[generatore_consumatore] ...............msg.array[1]: %d\n", pc->msg[pc->coda].ar[1]);
    printf("[generatore_consumatore] ...............msg.var: %d\n", pc->msg[pc->coda].var);
    message m;
    strcpy(m.string,pc->msg[pc->coda].string);
    m.ar[0]=pc->msg[pc->coda].ar[0];
    m.ar[1]=pc->msg[pc->coda].ar[1];
    m.var=pc->msg[pc->coda].var;
    
    int ret = msgsnd(ds_queue_gen_filter,&m,sizeof(message)-sizeof(long),0);
    
    if (ret<0){
            perror("msgsnd del messaggio on ds_queue_gen_filter FALLITA!");
            exit(-1);
    }
    printf("[generatore_consumatore] Messaggio INVIATO!\n");
   
    enter_monitor(&(pc->m)); 
    pc->coda=(pc->coda+1)%DIM_QUEUE;
    pc->totale_elementi--;
    signal_condition(&(pc->m),VARCOND_GEN_PRODUTTORI);
    leave_monitor(&(pc->m));
    exit(0);
}

