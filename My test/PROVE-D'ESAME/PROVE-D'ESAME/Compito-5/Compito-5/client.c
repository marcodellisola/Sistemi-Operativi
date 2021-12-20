#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#include "registro.h"

void client(int id_coda_registro_richieste, int id_coda_registro_risposte) {

    printf("Client: Avvio...\n");

    srand(getpid());

    int id_server = rand() % 2;

    printf("Client: Invio messaggio QUERY (id_server=%d)\n", id_server);

    /* TBD */
    messaggio_registro m;
    m.identificativo=id_server;
    m.type=QUERY;
    msgsnd(id_coda_registro_richieste,&m,sizeof(messaggio_registro)-sizeof(long),0);

    printf("Client: Attesa messaggio RESULT...\n");

    /* TBD */
    msgrcv(id_coda_registro_risposte,&m,sizeof(messaggio_registro)-sizeof(long),RESULT,0);

    int id_coda_server = /* TBD */m.id;

    printf("Client: Ricevuto messaggio RESULT (id_coda=%d)\n", id_coda_server);

    for(int i = 0; i<3; i++) {

        int valore = rand() % 11;

        printf("Client: Invio messaggio SERVICE (id_server=%d, id_coda=%d, valore=%d)\n", id_server, id_coda_server, valore);

        /* TBD */
	messaggio_server m1;
	m1.valore=valore;
	m1.type=SERVICE;
	msgsnd(id_coda_server,&m1,sizeof(messaggio_server)-sizeof(long),0);

        sleep(1);
    }

    printf("Client: Uscita\n");
}
