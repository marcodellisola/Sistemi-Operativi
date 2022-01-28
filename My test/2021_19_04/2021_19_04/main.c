#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/msg.h>

#include "auth.h"

int main() {

    printf("Creazione code di messaggi\n");

    int id_coda_auth_req = /* TBD */ msgget(IPC_PRIVATE, IPC_CREAT | 0664);

    int id_coda_auth_resp = /* TBD */ msgget(IPC_PRIVATE, IPC_CREAT | 0664);
    
    int id_coda_elab_req = /* TBD */ msgget(IPC_PRIVATE, IPC_CREAT | 0664);
    
    int id_coda_elab_resp = /* TBD */ msgget(IPC_PRIVATE, IPC_CREAT | 0664);


    pid_t pid;


    printf("Avvio processo autenticatore\n");

    /* TBD: Avvio processo (funzione Auth) */
    pid = fork();
    if(pid==-1){perror("Errore processo figlio!");exit(1);}
    if(pid==0){
	printf("Inizio Processo Auth!\n");
	Auth(id_coda_auth_req,id_coda_auth_resp);
	exit(0);
    }

    printf("Avvio processo server\n");

    /* TBD: Avvio processo (funzione Server) */
    pid = fork();
    if(pid==-1){perror("Errore processo figlio!");exit(1);}
    if(pid==0){
        printf("Inizio Processo Server!\n");
        Server(id_coda_elab_req,id_coda_elab_resp,id_coda_auth_req,id_coda_auth_resp);
        exit(0);
    }

    printf("Avvio processo client autenticato\n");

    /* TBD: Avvio processo (funzione ClientAuth) */
    pid = fork();
    if(pid==-1){perror("Errore processo figlio!");exit(1);}
    if(pid==0){
        printf("Inizio Processo ClientAuth!\n");
        ClientAuth(id_coda_auth_req,id_coda_auth_resp,id_coda_auth_req,id_coda_auth_resp);
        exit(0);
    }

    printf("Avvio processo client non-autenticato\n");

    /* TBD: Avvio processo (funzione ClientNonAuth) */
    pid = fork();
    if(pid==-1){perror("Errore processo figlio!");exit(1);}
    if(pid==0){
        printf("Inizio Processo ClientNonAuth!\n");
        ClientNonAuth(id_coda_elab_req,id_coda_elab_resp);
        exit(0);
    }


    printf("Attesa terminazione processi\n");

    /* TBD: Attesa terminazione */
    for(int i=0; i<4; i++){
	    wait(NULL);
	    printf("Processo %d terminato!\n");
    }

    printf("Rimozione code di messaggi\n");

    /* TBD: Rimozione code */ 
    msgctl(id_coda_auth_req,IPC_RMID,0);
    msgctl(id_coda_auth_resp,IPC_RMID,0);
    msgctl(id_coda_elab_req,IPC_RMID,0);
    msgctl(id_coda_elab_resp,IPC_RMID,0);

    return 0;
}
