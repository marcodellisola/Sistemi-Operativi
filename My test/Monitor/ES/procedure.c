#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include "header.h"

void Scrittore(meteo *p){
	for(int i=0;i<20;i++){
		enter_monitor(&(p->m));
		printf("Ingresso monitor - produzione!\n");
		if(p->buffer_lettori>0){
			printf("Produzione sospesa!\n");
			wait_condition(&(p->m), VARCOND_SCRITTORI);
			printf("Produzione riattivata!\n");
		}
		leave_monitor(&(p->m));
		p->umidita = rand() % 101;
		p->pioggia = rand() % 2;
		p->temperatura = (rand() % 101) - 50;
		
		enter_monitor(&(p->m));
		printf("La temperatura è di %d, l'umidità è del %d percento e la pioggia %d\n", p->temperatura,p->umidita,p->pioggia);
		signal_condition(&(p->m),VARCOND_LETTORI);
		leave_monitor(&(p->m));
		printf("Uscita monito!\n");
		sleep(2);
	}
}

void Lettore(meteo *p){
	for(int i=0;i<10;i++){
		enter_monitor(&(p->m));
		printf("Ingresso monitor - lettura!\n");
		wait_condition(&(p->m),VARCOND_LETTORI);
		p->buffer_lettori++;
		leave_monitor(&(p->m));
		printf("La temperatura è di %d, l'umidità è del %d percento e la pioggia %d\n", p->temperatura,p->umidita,p->pioggia);
		enter_monitor(&(p->m));
		p->buffer_lettori--;
		if(p->buffer_lettori==0){
			signal_condition(&(p->m), VARCOND_SCRITTORI);
		}
		leave_monitor(&(p->m));
		sleep(1);
	}



}
