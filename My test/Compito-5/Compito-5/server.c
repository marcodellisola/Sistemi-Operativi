#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "registro.h"

void server(int id_coda_registro_richieste, int id_coda_registro_risposte, int id_server) {
    
    printf("Registro: Server...\n");

    int risorsa = 0;

    int id_coda_server = /* TBD */msgget(IPC_PRIVATE, IPC_CREAT | 0664);

    printf("Server: Invio messaggio BIND (id_server=%d, id_coda=%d)\n", id_server, id_coda_server);

    /* TBD */
    messaggio_registro mr;
    mr.tipo=BIND;
    mr.id_server=id_server;
    mr.id_coda_server=id_coda_server;
    msgsnd(id_coda_registro_richieste,&mr,sizeof(messaggio_registro)-sizeof(long),0);


    while(1) {

        printf("Server: In attesa di messaggi...\n");

        /* TBD: Completare la ricezione e gestione dei messaggi */
	messaggio_server ms;
	msgrcv(id_coda_server,&ms,sizeof(messaggio_server)-sizeof(long),SERVICE,0);
	if(ms.tipo=EXIT){
		exit(0);
	}	
	risorsa=ms.valore;
    }
}
