#include "prodcons.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>


void inizializza(MonitorStreaming * p) {

    /* TBD: Inizializzare il monitor */
	init_monitor(&(p->m),2);
	p->num_liberi=DIM;
	p->num_occupati=0;
	p->elementi=0;
	for(int i=0; i<DIM ; i++){
		p->vettore[i].stato=LIBERO;
	}
}


void produci(MonitorStreaming * p, char * stringa, size_t lunghezza, key_t chiave) {

    /* TBD: Completare il metodo, con la sincronizzazione */
    int index=0;
    enter_monitor(&(p->m));
    while(p->num_liberi<=0){
	    wait_condition(&(p->m),VARCOND_PRODUTTORI);
    }
    while(index<=DIM && p->vettore[index].stato!=LIBERO){
	    index++;
    }
    p->vettore[index].stato=IN_USO;
    p->num_liberi--;

    printf("Avvio produzione...\n");

    sleep(1);
	
    int ds_shm = shmget(chiave,sizeof(char), IPC_CREAT | 0664);
    char * stringa_shm = /* TBD: Creare una nuova shared memory, su cui copiare la stringa */(char*) shmat(ds_shm,NULL,0);

    /* TBD */p->vettore[index].dimensione = lunghezza;
    /* TBD */p->vettore[index].chiave = chiave;
    /* TBD */p->vettore[index].produttore = getpid();

    p->vettore[index].stato=OCCUPATO;
    p->elementi++;
    p->num_occupati++;
    strcpy(stringa_shm, stringa);   // Copia della stringa

    printf("Produzione completata: %s (%zu char, key=%x)\n", stringa_shm, lunghezza, chiave);

    int shmdt(const void* shmaddr);
    signal_condition(&(p->m),VARCOND_CONSUMATORI);
    leave_monitor(&(p->m));
}

void consuma(MonitorStreaming * p, char * stringa, size_t * lunghezza) {

    /* TBD: Completare il metodo, con la sincronizzazione */
    int index=0;
    enter_monitor(&(p->m));
    while(p->num_occupati<=0){
	    wait_condition(&(p->m),VARCOND_CONSUMATORI);
    }
    while(index<=DIM && p->vettore[index].stato!=OCCUPATO){
	    index++;
    }
    p->vettore[index].stato=IN_USO;
    p->num_occupati--;

    printf("Avvio consumazione...\n");

    sleep(1);

    key_t chiave = /* TBD */p->vettore[index].chiave;
    pid_t produttore = /* TBD */p->vettore[index].produttore;
    *lunghezza = /* TBD */p->vettore[index].dimensione;

    p->vettore[index].stato=LIBERO;
    p->num_liberi++;
    p->elementi--;
    int ds_shm = shmget(chiave,sizeof(char), IPC_CREAT | 0664);
    char * stringa_shm = /* TBD: Accedere alla stringa su shared memory */(char*) shmat(ds_shm,NULL,0);

    strcpy(stringa, stringa_shm);   // Copia della stringa

    printf("Consumazione completata: %s (%zu char, key=%x)\n", stringa, *lunghezza, chiave);

    int shmdt(const void* shmaddr);
    signal_condition(&(p->m),VARCOND_PRODUTTORI);
    leave_monitor(&(p->m));
}

void distruggi(MonitorStreaming * p) {

    /* TBD: Completare il metodo */
	remove_monitor(&(p->m));
}
