#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#include "header.h"

void inizializza(MonitorCoda *p) {

    /* TBD: Aggiungere codice per l'inizializzazione del monitor e delle relative
     * variabili per la sincronizzazione
     */
    init_monitor(&(p->m),2);
    p->num_liberi=DIMENSIONE;
    p->num_occupati=0;
    p->produttori_wait=0;
    for(int i=0; i<DIMENSIONE; i++){
	    p->stato[i]=LIBERO;
    }
}

void rimuovi(MonitorCoda * p) {
    /* TBD: Aggiungere codice per la rimozione del monitor */
	remove_monitor(&(p->m));
}


int produzione(MonitorCoda *p, int valore) {

    /* TBD: Aggiungere codice per la produzione in accordo allo schema
     * con vettore di stato, ricordando che il valore di ritorno della funzione
     * indica se il monitor è sovraccarico o meno.
     * Nel caso in cui ci siano almeno 4 produttori in attesa, bisogna forzare
     * l'uscita del processo dal monitor e ritornare un valore pari ad 1.
     * Nel caso in cui la produzione avviene normalmente ritornare un valore pari a 0.
     */
	int index=0;
	
	enter_monitor(&(p->m));
	if(p->produttori_wait>=4){
                leave_monitor(&(p->m));
                return 1;
        }
	if(p->num_liberi==0){
		p->produttori_wait++;
		wait_condition(&(p->m),VARCOND_PRODUTTORI);
		p->produttori_wait--;
	}
	while(index<=DIMENSIONE && p->stato[index]!=LIBERO){
		index++;
	}
	p->stato[index]=IN_USO;
	p->num_liberi--;

	p->vettore[index]=valore;

	p->stato[index]=OCCUPATO;
	p->num_occupati++;
	signal_condition(&(p->m),VARCOND_CONSUMATORI);
	leave_monitor(&(p->m));

	return 0;
}


/* NOTA: il valore di ritorno di consumazione() è il valore prelevato dalla coda */

int consumazione(MonitorCoda *p) {


    /* TBD: Aggiungere codice per la consumazione in accordo allo schema
     * con vettore di stato, ricordando che il valore di ritorno della funzione
     * indica il valore prelevato dal buffer condiviso.
     */
	int index=0;
        int ret;
	enter_monitor(&(p->m));
	if(p->num_occupati==0){
		wait_condition(&(p->m),VARCOND_CONSUMATORI);
	}
	while(index<=DIMENSIONE && p->stato[index]!=OCCUPATO){
		index++;
	}
	p->stato[index]=IN_USO;
	p->num_occupati--;

	ret = p->vettore[index];

	p->stato[index]=LIBERO;
	p->num_liberi++;
	signal_condition(&(p->m),VARCOND_PRODUTTORI);
	leave_monitor(&(p->m));
	
	return ret;
}

void produttore(MonitorCoda *m) {
    
    int valore;
    int ret;
    int i;
    
    for(i=0; i<5; i++) {
        
        while(1) {
            
            srand(time(NULL)*getpid());
            
            valore = rand() % 10;
            
            ret = produzione(m, valore);
            
            if(ret==0) { break; }
            
            printf("MONITOR SOVRACCARICO, ATTENDO 3 secondi...\n");
            
            sleep(3);
        }
        
        printf("VALORE PRODOTTO: %d\n", valore);
        
        sleep(1);
    }
    
    
    exit(0);
}

void consumatore(MonitorCoda *m) {
    
    int i;
    int valore;
    
    for(i=0; i<15; i++) {
        
        valore = consumazione(m);
        
        printf("VALORE CONSUMATO: %d\n", valore);
        
        sleep(2);
    }
    
    
    exit(0);
}

