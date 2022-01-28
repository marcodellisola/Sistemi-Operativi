#include "prodcons.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void * Produttore(void *);
void * Consumatore(void *);

int main() {

    /* TBD: Avviare i thread produttore e consumatore */
	pthread_t threads[2];
	pthread_attr_t attr;
	pthread_attr_init;
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);

	MonitorStreaming *pc = (MonitorStreaming*) malloc(sizeof(MonitorStreaming));
	inizializza(pc);
	
	printf("Creazione thread produttore!\n");
	pthread_create(&threads[0],&attr,Produttore,(void*)pc);

	printf("Creazione thread consumatore!\n");
	pthread_create(&threads[1],&attr,Consumatore,(void*)pc);

	for(int i=0; i<2; i++){
		pthread_join(threads[i],NULL);
		printf("Threads %d terminato!\n",i);
	}

	distruggi(pc);
	pthread_attr_destroy(&attr);
	free(pc);

	return 0;
}

void * Produttore(void * p) {

	MonitorStreaming *m = (MonitorStreaming*) p;

    int tempo_attesa = 1000000;

    for(int i=0; i<12; i++) {

        Frame f;
        f[0][0] = rand() % 11;
        f[0][1] = rand() % 11;
        f[1][0] = rand() % 11;
        f[1][1] = rand() % 11;

        /* TBD: Chiamare il metodo "produci()" del monitor,
         *      incrementare "tempo_attesa" se il valore di ritorno è "1"
         */
	if(produci(m,f)==1){
		tempo_attesa+=500000;
	}

        usleep(tempo_attesa);
    }

    pthread_exit(NULL);
}

void * Consumatore(void * p) {

	MonitorStreaming *m = (MonitorStreaming*) p;

    for(int i=0; i<12; i++) {

        Frame f;

        /* TBD: Chiamare il metodo "consuma()" del monitor */
	consuma(m,f);
    }

    pthread_exit(NULL);
}
