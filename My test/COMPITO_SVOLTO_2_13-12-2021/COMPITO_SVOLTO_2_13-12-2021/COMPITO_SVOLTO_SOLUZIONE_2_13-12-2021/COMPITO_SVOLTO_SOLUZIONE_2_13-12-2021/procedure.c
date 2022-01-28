#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>

#include "header.h"

void inizializza(MonitorCoda *p) {
    
    init_monitor( &(p->m), 2);

    p->numero_liberi = DIMENSIONE;
	p->numero_elementi = 0;
	p->produttori_in_attesa = 0;

}

void rimuovi(MonitorCoda * p) {
    
    remove_monitor( &(p->m) );
}
    
/* NOTA: il valore di ritorno della funzione produzione()
 * indica se il monitor è sovraccarico
 */

int produzione(MonitorCoda *p, int valore) {

    enter_monitor( &(p->m) );

	if( p->produttori_in_attesa >= 4 ) {

		leave_monitor( &(p->m) );
		return 1;
	}

    if( p->numero_liberi == 0 ) {

		p->produttori_in_attesa++;

		printf("PRODUTTORI IN ATTESA: %d\n", p->produttori_in_attesa);

        wait_condition( &(p->m), CV_PRODUTTORI );
		p->produttori_in_attesa--;
	}

    int i = 0;
    while( i<DIMENSIONE && p->stato[i] != LIBERO ) {
        i++;
    }

    p->stato[i] = IN_USO;
    p->numero_liberi--;
    
    leave_monitor( &(p->m) );

	printf("Processo %d IN PRODUZIONE...\n", getpid());

	p->vettore[i] = valore;

    enter_monitor( &(p->m) );
    
    p->stato[i] = OCCUPATO;
	p->numero_elementi++;

    signal_condition( &(p->m), CV_CONSUMATORI );

	leave_monitor( &(p->m) );

	return 0;
}


/* NOTA: il valore di ritorno di consumazione() è il valore prelevato dalla coda */

int consumazione(MonitorCoda *p) {

	int valore;


	enter_monitor( &(p->m) );

    if ( p->numero_elementi == 0 ) {
	    wait_condition( &(p->m), CV_CONSUMATORI );
	}

    
    int i = 0;
    while( i<DIMENSIONE && p->stato[i] != OCCUPATO ) {
        i++;
    }
    
    p->stato[i] = IN_USO;
    p->numero_elementi--;
    
    leave_monitor( &(p->m) );
    
	printf("Processo %d IN CONSUMAZIONE...\n", getpid());

	valore = p->vettore[i];

    enter_monitor( &(p->m) );
    
    p->stato[i] = LIBERO;
    p->numero_liberi++;

	signal_condition( &(p->m), CV_PRODUTTORI );

	leave_monitor( &(p->m) );


	return valore;
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

