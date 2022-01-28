#include "header.h"



void inizializza(struct monitor* m){

	m->molo=0;
	m->id_nave=0;

    /* TODO: Inizializzare le variabili dell'algoritmo, il mutex, e le variabili condition */
	pthread_mutex_init(&(m->mutex),NULL);
	pthread_cond_init(&(m->ok_lett_cv),NULL);
	pthread_cond_init(&(m->ok_scritt_cv),NULL);
	m->num_lettori=0;
	m->num_scrittori=0;
}

void rimuovi (struct monitor* m){
    /* TODO: Disattivare mutex e variabili condition */
	pthread_mutex_destroy(&(m->mutex));
	pthread_cond_destroy(&(m->ok_lett_cv));
	pthread_cond_destroy(&(m->ok_scritt_cv));
}


//SCRITTURA. AGGIORNAMENTO DELLA POSIZIONE DEL TRENO
void scrivi_molo(struct monitor* m, int molo){

    /* TODO: Implementare qui lo schema dei lettori-scrittori con starvation di entrambi.
     * nella parte della SCRITTURA
     */
	pthread_mutex_lock(&(m->mutex));
	while(m->num_lettori>0 && m->num_scrittori==0){
		pthread_cond_wait(&(m->ok_scritt_cv),&(m->mutex));
	}
	m->num_scrittori++;

	m->molo=molo;

	m->num_scrittori--;
	if(m->num_scrittori==0){
		pthread_cond_signal(&(m->ok_lett_cv));
	}else{
		pthread_cond_signal(&(m->ok_scritt_cv));
	}
	pthread_mutex_unlock(&(m->mutex));
}


//LETTURA. RESTITUISCE LA POSIZIONE DEL TRENO
int leggi_molo(struct monitor* m){

    /* TODO: Implementare qui lo schema dei lettori-scrittori con starvation di entrambi.
     * nella parte della LETTURA
     */
	pthread_mutex_lock(&(m->mutex));
	while(m->num_scrittori>0){
		pthread_cond_wait(&(m->ok_lett_cv),&(m->mutex));
	}
	m->num_lettori++;

	int pos = m->molo;

	m->num_lettori--;
	if(m->num_lettori==0){
		pthread_cond_signal(&(m->ok_scritt_cv));
	}
	pthread_mutex_unlock(&(m->mutex));
	return pos;
}

