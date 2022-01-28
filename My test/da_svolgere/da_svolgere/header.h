#ifndef HEADER_H
#define HEADER_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>



struct monitor {

	int molo;
	int id_nave;

	/* TODO: Aggiungere variabili per la sincronizzazione */
	int num_scrittori;
	int num_lettori;
	pthread_mutex_t mutex;
	pthread_cond_t ok_scritt_cv;
	pthread_cond_t ok_lett_cv;
};

void inizializza(struct monitor * m);
void rimuovi (struct monitor * m);
void scrivi_molo(struct monitor * m, int molo);
int leggi_molo(struct monitor * m);



#endif
