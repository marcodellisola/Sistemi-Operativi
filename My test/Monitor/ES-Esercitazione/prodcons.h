#ifndef _PRODCONS_H_
#define _PRODCONS_H_

#include "monitor_hoare.h"

#define DIM 10

typedef struct {
    long vettore[DIM];
    int coda;
    int testa;
    int totale_elementi;

    Monitor m;
    
} ProdCons;

#define VARCOND_PRODUZIONE 0
#define VARCOND_CONSUMAZIONE 1

void inizializza(ProdCons * p);
void consuma(ProdCons * p);
void produci(ProdCons * p, int val);
void rimuovi(ProdCons * p);

#endif
