#include <pthread.h>

#ifndef HEADER_H
#define HEADER_H

#define DIM_BUFFER 5

/* TODO: Definire le macro per la comunicazione su coda sincrona */
#define OK_TO_SEND 1
#define REQUEST_TO_SEND 2
#define RISULTATO 3

extern int coda_RTS;
extern int coda_OTS;
extern int coda_risultati;

typedef struct {
    
    // pool di buffer condivisi
    int operandi[DIM_BUFFER];
    
    /* TODO: Aggiungere le variabili necessarie per la sincronizzazione */
    int num_occupati;
    int num_liberi;
    int coda;
    int testa;
    int elementi;
    pthread_mutex_t mutex;
    pthread_cond_t ok_prod_cv;
    pthread_cond_t ok_cons_cv;
    
} MonitorOperandi;

typedef struct {
    /* TODO: Definire la struttura messaggio */
	long tipo;
	int risultato;
} Risultato;

void inserisci_operando(MonitorOperandi * mo, int operando);
int *preleva_operando(MonitorOperandi * mo);

void inserisci_risultato(Risultato *m , int queue);
int preleva_risultato(Risultato *m, int queue, int tipomess);

void * genera_operandi(void *);
void * calcola(void *);
void preleva_risultati(int);

void initServiceQueues();
void removeServiceQueues();
    
#endif

