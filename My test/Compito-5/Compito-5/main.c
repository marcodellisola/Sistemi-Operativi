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
     int id_coda_registro_richieste = msgget(IPC_PRIVATE, IPC_CREAT | 0664);
     int id_coda_registro_risposte = msgget(IPC_PRIVATE, IPC_CREAT | 0664);


     for(int i=0; i<2; i++){
     	int pid = fork();
     	if(pid==-1){perror("Errore creazione processo figlio!");exit(1);}
     	if(pid==0){
		printf("Inizio processo server %d!\n",i);
		server(id_coda_registro_richieste,id_coda_registro_risposte,i);
		exit(0);
	}
     }

     int pid = fork();
     if(pid==-1){perror("Errore creazione processo figlio!");exit(1);}
     if(pid==0){
	     printf("Inizio processo registro!\n");
	     registro(id_coda_registro_richieste,id_coda_registro_risposte);
	     exit(0);
     }
         /* NOTA: I client sono avviati con un ritardo, per dare il tempo
             ai server di registrarsi sul processo registro.
     */

    sleep(2);

     for(int i=0; i<3; i++){
	     pid = fork();
	     if(pid==-1){perror("Errore creazione processo figlio!");exit(1);}
	     if(pid==0){
		     printf("Inizio processo client %d!\n",i);
		     client(id_coda_registro_richieste,id_coda_registro_risposte);
		     exit(0);
	     }
     }

    /* TBD: Far terminare il programma come indicato nella traccia */
    for(int i=0; i<3; i++){
	    wait(NULL);
	    printf("Processo client %d terminato!\n",i);
    }

    messaggio_registro mr;
    mr.tipo=EXIT;
    msgsnd(id_coda_registro_richieste,&mr,sizeof(messaggio_registro)-sizeof(long),0);

    msgctl(id_coda_registro_richieste,IPC_RMID,0);
    msgctl(id_coda_registro_risposte,IPC_RMID,0);

    return 0;
}
