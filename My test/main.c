#include "header.h"

/*Funzione che, per 10 volte,incrementa di uno la posizione molo, ovvero la posizione della nave, e tramite scrivi_molo effettua la modifica poi attende 3 secondi*/
void * gestoreMolo (void * p){
	struct monitor* m = (struct monitor*) p;
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

//Funzione che controlla la posizione di un nave per tre volte
void * Viaggiatori (void * p){
	struct monitor* m = (struct monitor*) p;
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
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
	
	srand(time(NULL));

	struct monitor* m[5];//l'array di monitor per gestire 5 navi

	pthread_t gestore_molo[5];//5 gestori molo
	pthread_t viagg[10];//10 viaggiatori	
	
	int i;

    /* TBD: Allocare 5 istanze di monitor, e attivarle con inizializza() */
	for(i=0; i<5; i++){
		m[i]= (struct monitor*) malloc(sizeof(struct monitor));
		inizializza(m[i]);
	}

	//assegno un id ad ogni nave
	m[0]->id_nave=1;
	m[1]->id_nave=2;
	m[2]->id_nave=3;
	m[3]->id_nave=4;
    	m[4]->id_nave=5;

    /* TBD: Avviare 5 thread, facendogli eseguire la funzione gestoreMolo(),
     *     e passando ad ognuno una istanza di monitor diversa m[i]
     */
	for(int k=0; k<5; k++){
		printf("Inizio thread gestore  n°: %d",k);
		int rc = pthread_create(&gestore_molo[k],&attr,gestoreMolo,(void *)m[k]);
		if(rc){
			printf("Errore creazione pthread!\n");
			exit(1);
		}
	}
    
    
    /* TBD: Avviare 10 thread, facendogli eseguire la funzione Viaggiatori(),
     *      e passando ad ognuno una istanza di monitor diversa, da scegliere
     *      a caso con "rand() % 5"
     */
	for(int j=0; j<10; j++){
		printf("Inizio thread viaggiatore n°: %d",j);
   		int r = rand()%5;
		int rc = pthread_create(&viagg[j],&attr,Viaggiatori,(void*)m[r]);
		if(rc){
			printf("Errpre creazione pthread!\n");
			exit(1);
		}
	}


    /* TBD: Effettuare la join con i thread "gestoreMolo" */
	for(int k=0; k<5; k++){
		pthread_join(gestore_molo[k],NULL);
		printf("Pthread n° %d terminato!\n",k);
	}
    
    /* TBD: Effettuare la join con i thread "Viaggiatori" */
	for(int k=0; k<10; k++){
		pthread_join(viagg[k],NULL);
		printf("Pthread n° %d terminato!\n",k);
	}
    
    /* TBD: Disattivazione delle 5 istanze di monitor con rimuovi() */
	for(int k=0; k<5; k++){
		rimuovi(m[k]);
	}
	pthread_attr_destroy(&attr);
    
    /* TBD: Deallocazione delle 5 istanze di monitor con free() */
	for(int k=0; k<5; k++){
		free(m[k]);
	}
	
    
	return 0;
}	
