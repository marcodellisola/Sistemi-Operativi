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
	messaggio_registro m;
	msgrcv(id_coda_registro_richieste,&m,sizeof(messaggio_registro)-sizeof(long),BIND,0);
	if(m.identificativo=0){
		id_code_server[0]=m.id;
	}else{
		id_code_server[1]=m.id;
	}
	msgrcv(id_coda_registro_richieste,&m,sizeof(messaggio_registro)-sizeof(long),QUERY,0);
	
	m.type=RESULT;
	if(m.identificativo==0){
		m.id=id_code_server[0];
		m.identificativo=0;
	}else{
		m.id=id_code_server[1];
		m.identificativo=1;
	}
	msgsnd(id_coda_registro_risposte,&m,sizeof(messaggio_registro)-sizeof(long),0);
    }
}
