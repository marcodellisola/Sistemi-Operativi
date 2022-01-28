#include "prodcons.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void Produttore(MonitorStreaming *p);
void Consumatore(MonitorStreaming *p);

int main() {

    /* TBD: Creare un oggetto monitor, e avviare i processi produttore e consumatore */
	int ds_shm = shmget(IPC_PRIVATE, sizeof(MonitorStreaming), IPC_CREAT | 0664);
	if(ds_shm<0){
		perror("Errore creazione shm!\n");
		exit(1);
	}

	MonitorStreaming *pc = (MonitorStreaming*) shmat(ds_shm,NULL,0);
	inizializza(pc);

	int pid = fork();
	if(pid==-1){perror("Errore creazione processo figlio!\n");exit(1);}
	if(pid==0){
		printf("Inizio processo produttore!\n");
		Produttore(pc);
		exit(0);
	}

	pid = fork();
	if(pid==-1){perror("Errore creazione processo figlio!\n");exit(1);}
	if(pid==0){
		printf("Inizio processo figlio!\n");
		Consumatore(pc);
		exit(0);
	}

	for(int i=0; i<2; i++){
		wait(NULL);
		printf("Processo figlio %d terminato!\n",i);
	}

	distruggi(pc);
	shmctl(ds_shm,IPC_RMID,0);

	return 0;
}

void Produttore(MonitorStreaming *p) {
	
    MonitorStreaming *m = (MonitorStreaming*) p;
    char stringa[20];
    size_t lunghezza;

    char char_chiave = 'a';
    key_t chiave;

    for(int i=0; i<10; i++) {

        lunghezza = 1 + rand() % 20;

        for(int j=0; j<lunghezza-1; j++) {
            stringa[j] = 97 + (rand()%26);
        }

        stringa[lunghezza-1] = '\0';


        chiave = /* TBD */ftok(".",char_chiave);

        char_chiave = char_chiave + 1;


        /* TBD: Chiamare il metodo "produci()" del monitor */
	produci(p,stringa,lunghezza,chiave);

        sleep(1);
    }
}

void Consumatore(MonitorStreaming *p) {

    MonitorStreaming *m = (MonitorStreaming*) p;
    char stringa[20];
    size_t lunghezza;

    for(int i=0; i<10; i++) {

        /* TBD: Chiamare il metodo "produci()" del monitor */
	    consuma(m,stringa,lunghezza);

        sleep(1);
    }
}
