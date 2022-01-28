#ifndef _HEADER_H_
#define _HEADER_H_

#include <pthread.h>

#define DIM 4

typedef int Frame[2][2];

typedef struct {

    Frame vettore[DIM];

    /* TBD: Completare la struttura dati monitor */
    int num_liberi;
    int num_occupati;
    int elementi;
    int testa;
    int coda;
    pthread_mutex_t mutex;
    pthread_cond_t ok_prod_cv;
    pthread_cond_t ok_cons_cv;

} MonitorStreaming;

void inizializza(MonitorStreaming * m);
int produci(MonitorStreaming * m, Frame f);
void consuma(MonitorStreaming * m, Frame f);
void flush(MonitorStreaming * m);
void distruggi(MonitorStreaming * m);

#endif
