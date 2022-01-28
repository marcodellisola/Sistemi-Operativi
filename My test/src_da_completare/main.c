#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "header.h"

int main(){

        pid_t pid;
        int ds_queue_gen_filter, ds_queue_filter_checksum, ds_queue_checksum_visual;
        int total_process = NUM_GENERATORS + NUM_FILTER + NUM_CHECKSUM + NUM_VISUAL;

        //create queues
        int key_queue_gen_filter = /* TODO: definire chiave coda generatore-filtro */ IPC_PRIVATE;
        int key_queue_filter_checksum = /* TODO: definire chiave coda filtro-checksum */ IPC_PRIVATE;
        int key_queue_checksum_visual = /* TODO: definire chiave coda checksum-visualizzatore */ IPC_PRIVATE;

        ds_queue_gen_filter = /* TODO: generare il descrittore coda generatore-filtro */ msgget(ds_queue_gen_filter, IPC_CREAT | 0664);
        ds_queue_filter_checksum = /* TODO: generare il descrittore coda filtro-checksum */ msgget(ds_queue_filter_checksum, IPC_CREAT | 0664);
        ds_queue_checksum_visual = /* TODO: generare il descrittore coda checksum-visualizzatore */ msgget(ds_queue_checksum_visual, IPC_CREAT | 0664);
        
        printf("[master] Code create...\n");
        printf("[master] ...........ds_queue_gen_filter: %d\n", ds_queue_gen_filter);
        printf("[master] ...........ds_queue_filter_checksum: %d\n", ds_queue_filter_checksum);
        printf("[master] ...........ds_queue_checksum_visual: %d\n", ds_queue_checksum_visual);

        /* creazione shm per prod-cons generatori */
    
        key_t shm_key = /* TODO: definire chiave shared memory per prod/cons generatori*/ IPC_PRIVATE;

        int ds_shm = /* TODO: generare il descrittore della shared memory per prod/cons generatori*/ shmget(IPC_PRIVATE, sizeof(struct ProdConsGen), IPC_CREAT | 0664);

        if(ds_shm<0) { perror("SHM errore"); exit(1); }

        /* TODO: ottenere il riferimento per il segmento di shm */
	struct ProdConsGen *pc = (struct ProdConsGen*) shmat(ds_shm, NULL,0);

        /* TODO: inizializzare variabili utilizzate per la sincronizzazione generatori */
       	pc->coda=0;
	pc->testa=0;
	pc->num_liberi=DIM_QUEUE;
	pc->num_occupati=0;
	pc->elementi=0;
	init_monitor(&(pc->m), 2);
    
        int i;

        for (i=0; i<NUM_GENERATORS; i++){
        
                pid = fork();

                if (pid == 0){
                
                    if (i % 2 == 0){
                        
                        printf("GENERATORE PROD PID: %d\n", getpid());
                        int j;
                        for (j=0; j<4; j++){
                            srand(time(NULL)*getpid()+j);
                            generatore_produttore(pc);
                        }
                        exit(0);
                    }
                    else {
                        printf("GENERATORE CONS PID: %d\n", getpid());
                        int j;
                        for (j=0; j<4; j++){
                            generatore_consumatore(pc, ds_queue_gen_filter);
                        }
                        exit(0);
                    }
                }        
        }        


        for (i=0; i<NUM_FILTER+NUM_CHECKSUM+NUM_VISUAL; i++){
                pid = fork();
                if (pid==0){
                        if (i==0){
                            printf("FILTRO PID: %d\n", getpid());
                            filtro(ds_queue_gen_filter, ds_queue_filter_checksum);
                        }
                        else if (i==1){
                            printf("CHECKSUM PID: %d\n", getpid());
                            checksum(ds_queue_filter_checksum, ds_queue_checksum_visual);
                        }
                        else if (i==2){
                            printf("VISUALIZZATORE PID: %d\n", getpid());
                            visualizzatore(ds_queue_checksum_visual);
                        }
                }
        }

       
        for (i=0; i<total_process; i++){
                wait(NULL);
        }

        /* TODO: deallocare risorse (code, shm, monitor) */
	msgctl(ds_queue_gen_filter, IPC_RMID,0);
	msgctl(ds_queue_filter_checksum, IPC_RMID,0);
	msgctl(ds_queue_checksum_visual, IPC_RMID,0);
	shmctl(ds_shm, IPC_RMID, 0);
	remove_monitor(&(pc->m));
        
        printf("[master] Rimozione code OK!\n");
        printf("[master] Rimozione monitor OK!\n");
    
        return 0;
}
