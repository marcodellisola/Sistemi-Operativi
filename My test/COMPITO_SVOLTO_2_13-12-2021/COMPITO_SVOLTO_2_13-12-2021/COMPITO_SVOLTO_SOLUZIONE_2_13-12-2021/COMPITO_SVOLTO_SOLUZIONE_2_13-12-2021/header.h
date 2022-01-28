#ifndef _HEADER_H_
#define _HEADER_H_

#include "monitor_hoare.h"

#define NUM_PRODUTTORI 6
#define NUM_CONSUMATORI 2

#define DIMENSIONE 5

#define CV_CONSUMATORI 0
#define CV_PRODUTTORI 1

#define LIBERO 0
#define OCCUPATO 1
#define IN_USO 2

typedef struct {

    int vettore[DIMENSIONE];
    int stato[DIMENSIONE];
    
    int numero_liberi;
    int numero_elementi;
	int produttori_in_attesa;

    Monitor m;
    
} MonitorCoda;

// il valore di ritorno di produzione() indica se il monitor è sovraccarico
int produzione(MonitorCoda *m, int valore);

// il valore di ritorno di consumazione() è il valore prelevato dalla coda
int consumazione(MonitorCoda *m);

void produttore(MonitorCoda *m);
void consumatore(MonitorCoda *m);

void inizializza(MonitorCoda *m);
void rimuovi(MonitorCoda * p);

#endif
