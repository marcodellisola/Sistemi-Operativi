#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <errno.h>

#include "header.h"

void checksum(int queue_filter_checksum, int queue_checksum_visual){

        int ret, i, j, checksum;
        message mess;

        for (j=0; j<NUM_MESSAGES; j++){

                printf("[checksum] Ricevo dal processo Filter...\n");

                ret = /* TODO: ricevere il messaggio dal processo filter */ msgrcv(queue_filter_checksum,&mess,sizeof(message)-sizeof(long),0,0);
            
                if(ret<0) {
                        if (errno == ENOMSG){
                                printf("Non ci sono più messaggi da ricevere dal processo filter...exit!\n");
                                break;
                        }
                        else{
                                perror("ERROR!!!");
                                exit(-1);
                        }
                }
                /* TODO: Calcolare la checksum e inviarla al visualizzatore  */
		if(mess.type==MSG_ERROR){
			printf("Impossibile elaborare messaggio!");
			msgsnd(queue_checksum_visual,&mess,sizeof(message)-sizeof(long),0);
			exit(1);
		}
		else{
			int valore;
			for(int i=0; i<STRING_MAX_DIM; i++){
				valore+=mess.stringa[i];
			}
			valore+=mess.arr[0];
			valore+=mess.arr[1];
			mess.var=valore;
			msgsnd(queue_checksum_visual,&mess,sizeof(message)-sizeof(long),0);
                
                printf("[checksum] Invio messaggio di CHECKSUM al Visualizzatore...\n");
        }
        
        exit(0);
}
}

