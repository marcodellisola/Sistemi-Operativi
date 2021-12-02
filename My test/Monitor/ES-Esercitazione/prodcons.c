#include <stdio.h>
#include <unistd.h>

#include "prodcons.h"

void inizializza(ProdCons * p) {
	init_monitor(&(p->m), DIM);
}

void consuma(ProdCons * p) {
    enter_monitor(&(p->m));
    printf("[%d] Ingresso consumatore\n", getpid());

    if((p->totale_elementi)<4){
	    printf("Consumazione in attesa!\n");
	    wait_condition(&(p->m), VARCOND_CONSUMAZIONE);
	    printf("Ripresa consumazione!\n");
    }
    for(int i=0; i<3; i++){
	    for(int j=0; j<3; j++){
    		printf("Il valore consumato è: %ld\n",p->vettore[p->coda]);
    		p->coda = (p->coda +1) % DIM;
		p->totale_elementi--;
		sleep(1);
		}
    }

    signal_condition(&(p->m), VARCOND_PRODUZIONE);
    printf("[%d] Uscita consumatore\n", getpid());
    leave_monitor(&(p->m));
}

void produci(ProdCons * p, int val) {

    enter_monitor(&(p->m));
    printf("[%d] Ingresso produttore\n", getpid());

    if((p->totale_elementi)==10){
	    printf("Produzione in attesa!\n");
	    wait_condition(&(p->m), VARCOND_PRODUZIONE);
	    printf("Ripresa produzione!\n");
    }
    
    p->vettore[p->testa] = val;
    p->testa = (p->testa + 1) % DIM;
    p->totale_elementi++;

    printf("[%d] Produzione: val=%d\n", getpid(), val);

     if((p->totale_elementi)>=3){
	     signal_condition(&(p->m),VARCOND_CONSUMAZIONE);
     }
     sleep(2);
    printf("[%d] Uscita produttore\n", getpid());
    leave_monitor(&(p->m));
}

void rimuovi(ProdCons * p) {
	remove_monitor(&(p->m));
}
