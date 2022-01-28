#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>

#include "header.h"




int main() {

    int shm_id = shmget(IPC_PRIVATE, sizeof(MonitorCoda), IPC_CREAT | 0644);
    
    if (shm_id < 0)
    {
        perror("Errore creazione shared memory");
        exit(1);
    }
    
    MonitorCoda *p = (MonitorCoda *) shmat(shm_id, NULL, 0);
    
    if (p == (void *)-1)
    {
        perror("Errore attach shared memory");
        exit(1);
    }

    inizializza(p);



	int i;
    pid_t pid;
    
	for(i=0; i<NUM_PRODUTTORI; i++) {
        pid = fork();
        if (pid == 0)
            produttore(p);
	}

    for(i=0; i<NUM_CONSUMATORI; i++) {
        pid = fork();
        if (pid == 0)
            consumatore(p);
    }

	for(i=0; i<NUM_PRODUTTORI; i++) {

        wait(NULL);
        printf("Terminato Processo\n");
	}


    for(i=0; i<NUM_CONSUMATORI; i++) {
        
        wait(NULL);
        printf("Terminato Processo\n");
    }
    
	rimuovi(p);
    
    return 0;
}

