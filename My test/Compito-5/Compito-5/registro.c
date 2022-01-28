#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "registro.h"

void registro(int id_coda_registro_richieste, int id_coda_registro_risposte) {
    
    printf("Registro: Avvio...\n");

    int id_code_server[2];
    id_code_server[0] = 0;
    id_code_server[1] = 0;

    while(1) {

        printf("Registro: In attesa di messaggi...\n");

         /* TBD: Completare la ricezione e gestione dei messaggi */
	messaggio_registro mr1,mr2;
	msgrcv(id_coda_registro_richieste,&mr1,sizeof(messaggio_registro)-sizeof(long),0,0);
	if(mr1.tipo==BIND){
		id_code_server[mr1.id_server]=mr1.id_coda_server;
	}
       	if(mr1.tipo==QUERY){
		mr2.tipo=RESULT;
		mr2.id_server=mr1.id_server;
		mr2.id_coda_server=id_code_server[mr1.id_server];
		msgsnd(id_coda_registro_risposte,&mr2,sizeof(messaggio_registro)-sizeof(long),0);

        }
	if(mr1.tipo==EXIT){
		messaggio_server ms;	
		ms.tipo=EXIT;
		for(int i=0; i<2; i++){
			msgsnd(id_code_server[i],&ms,sizeof(messaggio_server)-sizeof(long),0);
		}
		exit(0);
	}
    }
}
