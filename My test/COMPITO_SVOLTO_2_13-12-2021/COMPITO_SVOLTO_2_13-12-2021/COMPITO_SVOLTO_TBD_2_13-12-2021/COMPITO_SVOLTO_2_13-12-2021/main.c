#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#include "header.h"

int main() {
    int pid;
    int shm_id = /* TBD: Ottenere un descrittore della shared memory */shmget(IPC_PRIVATE,sizeof(MonitorCoda),IPC_CREAT | 0664);
    
    if (shm_id < 0)
    {
        perror("Errore creazione shared memory");
        exit(1);
    }
    
    MonitorCoda *p = /* TBD: Effettuare l'attache della shared memory */(MonitorCoda*) shmat(shm_id,NULL,0);
    
    if (p == (void *)-1)
    {
        perror("Errore attach shared memory");
        exit(1);
    }

    /* TBD: Inizializzare monitor e variabili per la sincronizzazione tramite la procedura inizializza(...) */
    inizializza(p);
    /* TBD: Aggiungere codice per la creazione dei processi produttori e consumatori */
    for(int i=0; i<NUM_PRODUTTORI; i++){
    	pid = fork();
    	if(pid==-1){perror("Errore processo figlio!");exit(1);}
    	if(pid==0){
	    	printf("Inizio processo produttore!\n");
	    	produttore(p);
	    	exit(0);
    	}
    }
	
    for(int i=0; i<NUM_CONSUMATORI; i++){
    	pid = fork();
    	if(pid==-1){perror("Errore processo figlio!");exit(1);}
    	if(pid==0){
	    	printf("Inizio processo consumatore!\n");
	    	consumatore(p);
	    	exit(0);
    	}
    }

    /* TBD: Aggiungere codice per l'attesa della terminazione dei processi produttori e consumatori */
    for(int i=0; i<NUM_PRODUTTORI+NUM_CONSUMATORI; i++){
	    wait(NULL);
	    printf("Processo figlio terminato!\n");
    }
    
    /* TBD: Aggiungere codice per la rimozione del monitor tramite la procedura rimuovi(...) */
    rimuovi(p);
    shmctl(shm_id,IPC_RMID,0);
    
    return 0;
}

