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
	pthread_mutex_init(&(b->mutex),NULL);
	pthread_cond_init(&(b->ok_cons_cv),NULL);
	b->num_mess=0;
	printf("[SERVER] - Buffer inizializzato...\n");
}

void remove_buffer(Buffer* b){
	//TODO distruzione delle variabili de buffer
 	pthread_mutex_destroy(&(b->mutex));
	pthread_cond_destroy(&(b->ok_cons_cv));	
	printf("[SERVER] - Buffer distrutto...\n");
}


void aggiorna_somma(Buffer* b, int somma_parziale){
	// TODO aggiungere la somma parziale alla somma nel buffer,
	//		risvegliando il prelievo della somma finale quando necessario...
	pthread_mutex_lock(&(b->mutex));

	b->somma+=somma_parziale;
	b->num_mess++;
	if(b->num_mess==NUM_WORKER*NUM_REQ){
		pthread_cond_signal(&(b->ok_cons_cv));
	}
	
	pthread_mutex_unlock(&(b->mutex));
}

int preleva_somma_finale(Buffer* b){
	int somma_finale;
	//TODO prelevare la somma finale dal buffer, 
	// 		solo quando sono state accumulati NUM_WORKER*NUM_REQ incrementi
	pthread_mutex_lock(&(b->mutex));
	if(b->num_mess<NUM_WORKER*NUM_REQ){
		pthread_cond_wait(&(b->ok_cons_cv),&(b->mutex));
	}
	somma_finale=b->somma;
	pthread_mutex_unlock(&(b->mutex));

	return somma_finale;
}

void* collector(void* arg){
	int somma_finale;
	float media;

	// TODO recupera l'argomento
	Buffer *m = (Buffer*) arg;

	printf("[COLLECTOR] - start del thread...\n");

	// TODO preleva la somma chiamando preleva_somma_finale
	somma_finale=preleva_somma_finale(m);
	media = somma_finale/30;
	
	printf("[COLLECTOR] - somma finale: %d media: %f\n",somma_finale,media);
	
}

void *worker(void* arg){
	static int c = 0;
	c++;	
	printf("[WORKER] - start del thread...\n");

	//TODO recupera i parametri
	Buffer *p = (Buffer*) arg;
	Misura m;
	for(int i=0; i<NUM_REQ; i++){
		msgrcv(coda,&m,sizeof(Misura)-sizeof(long),c,0);
		aggiorna_somma(p,m.misura);
	}
	
	//TODO ogni worker preleva NUM_REQ messaggi relativi ad un certo client id 
	//     e per ogni messaggio ricevuto aggiorna la somma chiamando aggiorna_somma
	

}



int main(){

	int i,status;
	pid_t pid;
	
	pthread_t threads[4];
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

	key_t key;
	key = ftok(".",'a');
    
	coda = msgget(key,0);

	printf("[SERVER] - id coda %d\n",coda);


	//Inizializziamo strutture
	
	Buffer* b = (Buffer*) malloc(sizeof(Buffer));

	init_buffer(b);
	
	
	//Avvio dei thread
	for(i = 0; i < NUM_WORKER+1;i++){

		printf("[MASTER] - creo thread %d...\n",i);

		// TODO creazione di 1 trhead collector e 3 thread worker (ai quali passare anche l'id)
		if(i==0){
			printf("Inizio thread collector!\n");
			pthread_create(&threads[i],&attr,collector,(void*)b);
		}else{
			printf("Inizio threads worker %d!\n",i);
			pthread_create(&threads[i],&attr,worker,(void*)b);
		}
		
	}

	for(int i=0; i<NUM_WORKER+1; i++){
		pthread_join(threads[i],NULL);
		printf("Thread terminato!");
	}

	printf("[MASTER] - bye bye...\n");
	pthread_attr_destroy(&attr);
	remove_buffer(b);
	free(b);
	pthread_exit(NULL);

    return 0;
}

