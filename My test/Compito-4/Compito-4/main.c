#include "sensore.h"
#include "aggregatore.h"
#include "collettore.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {


    /* TBD: Creare le code di messaggi, 
     * e avviare i processi sensore, aggregatore, e collettore 
     */
   int queue_sens_agg = msgget(IPC_PRIVATE,IPC_CREAT | 0664);
   int queue_agg_coll[3];
   for(int i=0; i<3; i++){
   	queue_agg_coll[i] = msgget(IPC_PRIVATE,IPC_CREAT | 0664);
   }
	
   int pid = fork();
   if(pid==-1){perror("Errore creazione processo figlio!");exit(1);}
   if(pid==0){
	   printf("Inizio processo sensore!\n");
           sensore(queue_sens_agg);
	   exit(1);
   }

   pid = fork();
   if(pid==-1){perror("Errore creazione processo figlio!");exit(1);}
   if(pid==0){
	printf("Inizio processo aggregatore!\n");
	aggregatore(queue_sens_agg,queue_agg_coll);
	exit(0);
   }

   for(int i=0; i<3; i++){
	   pid = fork();
	   if(pid==-1){perror("Errore creazione processo figlio!");exit(1);}
	   if(pid==0){
		printf("Inizio processo collettore!\n");
		collettore(queue_agg_coll[i]);
		exit(0);
	   }
   }

   for(int i=0; i<5; i++){
	wait(NULL);
     	printf("Processo %d terminato!\n");
   }
 
   msgctl(queue_sens_agg,IPC_RMID,0);
   for(int i=0; i<3; i++){
	msgctl(queue_agg_coll[i],IPC_RMID,0);  
   }
 
   return 0;	
}
