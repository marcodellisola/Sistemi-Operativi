#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "prodcons.h"

void inizializza(MonitorPC * m) {

    printf("Inizializzazione monitor\n");

    /* TBD: Inizializzazione */
    pthread_mutex_init(&(m->mutex),NULL);
    pthread_cond_init(&(m->ok_cons_cv),NULL);
    pthread_cond_init(&(m->ok_prod_cv),NULL);
    m->num_liberi=4;
    m->num_occupati_tipo1=0;
    m->num_occupati_tipo2=0;
    for(int i=0; i<DIM; i++){
	    m->stato[i]=LIBERO;
    }
}


void rimuovi(MonitorPC * m) {

    printf("Rimozione monitor\n");

    /* TBD: Rimozione */
    pthread_mutex_destroy(&(m->mutex));
    pthread_cond_destroy(&(m->ok_cons_cv));
    pthread_cond_destroy(&(m->ok_prod_cv));
}


void produci_tipo_1(MonitorPC * m, int valore) {

    int index = 0;


    /* TBD: Sincronizzazione, e selezione dell'indice su cui produrre */
	pthread_mutex_lock(&(m->mutex));
	
	while(index<DIM && m->stato[index]!=LIBERO){
		pthread_cond_wait(&(m->ok_prod_cv),&(m->mutex));
		index++;
	}
	m->stato[index]=INUSO;

    printf("Produzione di tipo 1 in corso (valore=%d, index=%d)\n", valore, index);

    sleep(1);
    m->vettore[index] = valore;

    m->stato[index]=OCCUPATO1;

    pthread_cond_signal(&(m->ok_cons_cv));
    pthread_mutex_unlock(&(m->mutex));
}


void produci_tipo_2(MonitorPC * m, int valore) {

	int index = 0;


    /* TBD: Sincronizzazione, e selezione dell'indice su cui produrre */
        pthread_mutex_lock(&(m->mutex));
        
        while(index<DIM && m->stato[index]!=LIBERO){
                pthread_cond_wait(&(m->ok_prod_cv),&(m->mutex));
                index++;
        }
        m->stato[index]=INUSO;

    printf("Produzione di tipo 1 in corso (valore=%d, index=%d)\n", valore, index);

    sleep(1);
    m->vettore[index] = valore;

    m->stato[index]=OCCUPATO2;

    pthread_cond_signal(&(m->ok_cons_cv));
    pthread_mutex_unlock(&(m->mutex));
}


void consuma_tipo_1(MonitorPC * m, int valore) {

    int index = 0;


    /* TBD: Sincronizzazione, e selezione dell'indice su cui consumare */
	pthread_mutex_lock(&(m->mutex));

	while(index<DIM && m->stato[index]!=OCCUPATO1){
		pthread_cond_wait(&(m->ok_cons_cv),&(m->mutex));
		index++;
	}
	m->stato[index]=INUSO;

    sleep(1);
    valore = m->vettore[index];

    printf("Consumazione di tipo 1 (valore=%d, index=%d)\n", valore, index);
	m->stato[index]=LIBERO;

	pthread_cond_signal(&(m->ok_prod_cv));
	pthread_mutex_unlock(&(m->mutex));
}


void consuma_tipo_2(MonitorPC * m, int valore) {

       int index = 0;


    /* TBD: Sincronizzazione, e selezione dell'indice su cui consumare */
        pthread_mutex_lock(&(m->mutex));

        while(index<DIM && m->stato[index]!=OCCUPATO2){
                pthread_cond_wait(&(m->ok_cons_cv),&(m->mutex));
                index++;
        }
        m->stato[index]=INUSO;

    sleep(1);
    valore = m->vettore[index];

    printf("Consumazione di tipo 1 (valore=%d, index=%d)\n", valore, index);
        m->stato[index]=LIBERO;

        pthread_cond_signal(&(m->ok_prod_cv));
        pthread_mutex_unlock(&(m->mutex));

}
