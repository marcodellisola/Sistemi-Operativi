#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

#include "registro.h"

int main() {


    /* TBD: Creare le code di messaggi, e avviare i processi server e registro,
     *      facendogli chiamare le funzioni server() e registro().
     */


    /* NOTA: I client sono avviati con un ritardo, per dare il tempo
             ai server di registrarsi sul processo registro.
     */
	int id_coda_registro_richieste = msgget(IPC_PRIVATE, IPC_CREAT | 0664);
	int id_coda_registro_risposte = msgget(IPC_PRIVATE, IPC_CREAT | 0664);

	int pid;
	for(int i=0; i<2; i++){
		pid=fork();
		if(pid==-1){perror("Errore processo figlio!");exit(1);}
		if(pid==0){
			printf("Inizio processo server!\n");
			int id_server = i;
			server(id_coda_registro_richieste,id_coda_registro_risposte,id_server);
		}	
	}
	pid = fork();
	if(pid==-1){perror("Errore processo figlio!");exit(1);}
	if(pid==0){
		printf("Inizio processo registro!\n");
		registro(id_coda_registro_richieste,id_coda_registro_risposte);
	}

    sleep(2);

    /* TBD: Avviare i processi client, facendogli chiamare la funzione client() */
    for(int i=0; i<3; i++){
	    pid = fork();
	    if(pid==-1){perror("Errore processo figlio!");exit(1);}
	    if(pid==0){
    		printf("Inizio processo client!\n");
		client(id_coda_registro_richieste,id_coda_registro_risposte);
		exit(0);
	    }
    }
	

    /* TBD: Far terminare il programma come indicato nella traccia */
    for(int i=0; i<3; i++){
	    wait(NULL);
	    printf("Processo figlio terminato!");
    }

    msgctl(id_coda_registro_richieste,IPC_RMID,0);
    msgctl(id_coda_registro_risposte,IPC_RMID,0);

    kill(0,SIGINT);

    return 0;
}
