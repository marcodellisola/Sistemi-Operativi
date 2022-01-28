#include "prodcons.h"

#include <stdio.h>


void inizializza(MonitorStreaming * m) {

    /* TBD: Inizializzare il monitor */
	pthread_mutex_init(&(m->mutex),NULL);
	pthread_cond_init(&(m->ok_cons_cv),NULL);
	pthread_cond_init(&(m->ok_prod_cv),NULL);
	m->num_liberi=4;
	m->num_occupati=0;
	m->elementi=0;
	m->testa=0;
	m->coda=0;
}

int produci(MonitorStreaming * m, Frame f) {

    /* TBD: Completare il metodo, con la sincronizzazione */
    int attesa = /* TBD */0;

    pthread_mutex_lock(&(m->mutex));
    while(m->num_liberi>0){
    	pthread_cond_wait(&(m->ok_prod_cv),&(m->mutex));
	attesa=1;
    }
    m->num_liberi--;
   
    printf("Produzione frame: { %d, %d, %d, %d }\n", f[0][0], f[0][1], f[1][0], f[1][1]);

    /* TBD */m->vettore[m->testa][0][0] = f[0][0];
    /* TBD */m->vettore[m->testa][0][1] = f[0][1];
    /* TBD */m->vettore[m->testa][1][0] = f[1][0];
    /* TBD */m->vettore[m->testa][1][1] = f[1][1];

    m->testa = (m->testa + 1) % DIM;
    m->elementi++;
    m->num_occupati++;
    pthread_cond_signal(&(m->ok_cons_cv));
    pthread_mutex_unlock(&(m->mutex));

    return attesa;
}

void consuma(MonitorStreaming * m, Frame f) {

    /* TBD: Completare il metodo, con la sincronizzazione */
	pthread_mutex_lock(&(m->mutex));
	while(m->num_occupati<0){
		pthread_cond_wait(&(m->ok_cons_cv),&(m->mutex));
	}
	m->num_occupati--;

    f[0][0] = /* TBD */m->vettore[m->coda][0][0];
    f[0][1] = /* TBD */m->vettore[m->coda][0][1];
    f[1][0] = /* TBD */m->vettore[m->coda][1][0];
    f[1][1] = /* TBD */m->vettore[m->coda][1][1];

    printf("Consumazione frame: { %d, %d, %d, %d }\n", f[0][0], f[0][1], f[1][0], f[1][1]);

    m->coda = (m->coda + 1) % DIM;
    m->elementi--;
    m->num_liberi++;
    pthread_cond_signal(&(m->ok_prod_cv));
    pthread_mutex_unlock(&(m->mutex));
}

void flush(MonitorStreaming * m) {

    /* TBD: Completare il metodo */
	pthread_mutex_lock(&(m->mutex));
	m->testa=0;
	m->coda=0;
	m->elementi=0;
	m->num_liberi=4;
	m->num_occupati=0;
	pthread_cond_signal(&(m->ok_prod_cv));
	pthread_mutex_unlock(&(m->mutex));
}

void distruggi(MonitorStreaming * m) {

    /* TBD: Completare il metodo */
	pthread_mutex_destroy(&(m->mutex));
	pthread_cond_destroy(&(m->ok_cons_cv));
	pthread_cond_destroy(&(m->ok_prod_cv));
}
