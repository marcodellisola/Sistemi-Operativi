#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "header.h"

#define NUM_WORKER 3

static int coda;
static pthread_t threads[NUM_WORKER+1];


// struttura per il passaggio dei parametri ai thread worker
typedef struct {
	int id;
	Buffer* b;
} parametri;

void init_buffer(Buffer* b){
	//TODO inizializzazione del buffer
	pthread_mutex_init ( &(b->mutex), NULL );

	b->somma = 0;
	b->count = 0;

	printf("[SERVER] - Buffer inizializzato...\n");
}

void remove_buffer(Buffer* b){
	//TODO distruzione delle variabili de buffer
	pthread_mutex_destroy ( &(b->mutex) );

	free ( b );
	
	printf("[SERVER] - Buffer distrutto...\n");
}


void aggiorna_somma(Buffer* b, int somma_parziale){
	// TODO aggiungere la somma parziale alla somma nel buffer,
	//		risvegliando il prelievo della somma finale quando necessario...
	pthread_mutex_lock ( &(b->mutex) );

	if ( b->count == NUM_WORKER*NUM_REQ ) {
		preleva_somma_finale ( b );
	} else {
		b->somma = b->somma + somma_parziale;
		b->count++;
	}

	pthread_mutex_unlock ( &(b->mutex) );

}

int preleva_somma_finale(Buffer* b){
	int somma_finale;
	//TODO prelevare la somma finale dal buffer, 
	// 		solo quando sono state accumulati NUM_WORKER*NUM_REQ incrementi
	pthread_mutex_lock ( &(b->mutex) );
	somma_finale = b->somma;
	pthread_mutex_unlock ( &(b->mutex) );

	return somma_finale;
}

void* collector(void* arg){
	int somma_finale;
	float media;

	// TODO recupera l'argomento
	parametri * p = (parametri *) arg;

	printf("[COLLECTOR] - start del thread...\n");

	// TODO preleva la somma chiamando preleva_somma_finale
	while ( p->b->count < NUM_REQ * NUM_WORKER ) {
		sleep (1);
	}
	
	somma_finale = p->b->somma;
	
	media = somma_finale / (NUM_WORKER * NUM_REQ);
	//TODO calcola la media sul totale delle misure ricevute
	
	printf("[COLLECTOR] - somma finale: %d media: %f\n",somma_finale,media);
	
}

void *worker(void* arg){

	printf("[WORKER] - start del thread...\n");

	//TODO recupera i parametri
	parametri * p = (parametri *) arg;

	Misura m;
	int cont;
	
	//TODO ogni worker preleva NUM_REQ messaggi relativi ad un certo client id 
	//     e per ogni messaggio ricevuto aggiorna la somma chiamando aggiorna_somma
	while ( cont < NUM_REQ ) {

		msgrcv ( coda, &m, sizeof(Misura)-sizeof(long), 0, 0 );
		printf("Ricevuto msg numero %d, contiene %d\n", cont, m.val );
		aggiorna_somma ( p->b, p->id );
		cont++;

	}
	
	printf("worker finito\n");

}



int main(){

	int i,status;
	pid_t pid;

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	key_t key;
	key = ftok ( PATH, 'b' );
	//TODO:inserire la chiave
    
	coda = msgget ( key, IPC_CREAT | 0644 );
	//TODO:inizializzare la coda

	printf("[SERVER] - id coda %d\n",coda);


	//Inizializziamo strutture
	
	Buffer* b = (Buffer *) malloc (sizeof(Buffer));
	parametri * p = (parametri *) malloc (sizeof(parametri));
	//...

	init_buffer(b);
	
	
	//Avvio dei thread
	for(i = 0; i < NUM_WORKER+1;i++){

		printf("[MASTER] - creo thread %d...\n",i);

		// TODO creazione di 1 trhead collector e 3 thread worker (ai quali passare anche l'id)
		if ( i = NUM_WORKER-1 ) {
			pthread_create ( &threads[i], &attr, collector, (void *) p );
		} else {
			pthread_create ( &threads[i], &attr, worker, (void *) p );
		}
		
	}

	remove_buffer ( b );
	printf("[MASTER] - bye bye...\n");
	pthread_attr_destroy(&attr);
	pthread_exit(NULL);

    return 0;
}

