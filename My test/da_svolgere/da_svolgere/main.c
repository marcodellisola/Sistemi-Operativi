#include "header.h"

/* Funzione che, per 10 volte,incrementa di uno la posizione molo, ovvero la posizione della nave, e tramite scrivi_molo effettua la modifica poi attende 3 secondi*/
void * gestoreMolo (void * p){
	
    // TODO: ottenere il riferimento alla struttura monitor da *p
    struct monitor * m = (struct monitor*) p;
    
	int i,s;
	s=m->molo;
	for(i=0; i<10; i++){
        
		s=s+1;
		scrivi_molo(m,s);
		printf("[C: Posizione della nave %d modificata. La nuova posizione è: %d]\n",m->id_nave,s);
		sleep(3);
        
	}
	pthread_exit(NULL);
}

// Funzione che controlla la posizione di un nave per tre volte
void * Viaggiatori (void * p){
	
    // TODO: ottenere il riferimento alla struttura monitor da *p
    struct monitor *m = (struct monitor*) p;
    
	int i;
	int ris;
	for(i=0;i<3;i++){
        
		sleep(rand()%6+1);
		ris=leggi_molo(m);
		printf("[V]~La nave n.ro %d è nel molo %d\n", m->id_nave, ris);
        
	}
	pthread_exit(NULL);
}

int main() {
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	
	srand(time(NULL));

	struct monitor* m[5];//l'array di monitor per gestire 5 navi

	pthread_t gestore_molo[5];//5 gestori molo
	pthread_t viagg[10];//10 viaggiatori	
	
	int i;

    /* TODO: Allocare 5 istanze di monitor, e attivarle con inizializza() */
	for(i=0; i<5; i++){
		m[i] = (struct monitor*) malloc(sizeof(struct monitor));
		inizializza(m[i]);
	}

	// TODO: assegnare un id ad ogni nave
	for(i=0; i<5; i++){
		m[i]->id_nave=i+1;
	}

    /* TODO: Avviare 5 thread, facendogli eseguire la funzione gestoreMolo(),
     *     e passando ad ognuno una istanza di monitor diversa m[i]
     */
	for(i=0; i<5; i++){
		printf("Inizio thread gestore: %d",i);
		int rc = pthread_create(&gestore_molo[i],&attr,gestoreMolo,(void*)m[i]);
		if(rc){
			printf("Errore creazione pthread!\n");
			exit(1);
		}
	}
    
    
    /* TODO: Avviare 10 thread, facendogli eseguire la funzione Viaggiatori(),
     *      e passando ad ognuno una istanza di monitor diversa, da scegliere
     *      a caso con "rand() % 5"
     */
	for(i=0; i<10; i++){
		printf("Inizio thread viaggiatore!\n");
		int rc = pthread_create(&viagg[i],&attr,Viaggiatori,(void*)m[rand()%5]);
		if(rc){
			printf("Errore creazione pthread!\n");
			exit(1);
		}
	}
    
    
    /* TODO: Effettuare la join con i thread "gestoreMolo" */
    for(i=0; i<5; i++){
	    pthread_join(gestore_molo[i],NULL);
	   printf("Pthread gestore %d terminato!\n",i);
    }
    /* TODO: Effettuare la join con i thread "Viaggiatori" */
    for(i=0; i<10; i++){
	    pthread_join(viagg[i],NULL);
	    printf("Pthread viaggiatore %d terminato!\n");
    }
    /* TODO: Disattivazione delle 5 istanze di monitor con rimuovi() */
    for(i=0; i<5; i++){
	    rimuovi(m[i]);
    }
    pthread_attr_destroy(&attr);
    
    /* TODO: Deallocazione delle 5 istanze di monitor con free() */
    for(i=0; i<5; i++){
	    free(m[i]);
    }	
    
	return 0;
}	
