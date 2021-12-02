#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include "header.h"

void Generatore(int queue){
	Pipeline p;
	for(int i=0; i<2; i++){
		p.tipo=MESSAGGIO;
		int c=rand()%10+1;
		for(int i=0; i<c; i++){
			p.string[i]='a'+(rand()%26);
		}
		p.string[c]='\0';
		for(int i=0; i<2; i++){
			p.ar[i]=rand()%10;
		}
		p.var=0;

		msgsnd(queue,&p,sizeof(Pipeline)-sizeof(long),0);
	}
}

void Filtro(int queue1, int queue2){
	Pipeline p;
	for(int i=0; i<6; i++){
		msgrcv(queue1,&p,sizeof(Pipeline)-sizeof(long),MESSAGGIO,0);
		if(strchr(p.string,'x')==NULL){
			msgsnd(queue2,&p,sizeof(Pipeline)-sizeof(long),0);
		} else{
			p.tipo=MSG_ERROR;
			msgsnd(queue2,&p,sizeof(Pipeline)-sizeof(long),0);
			}
	}
}

void Checksum(int queue1, int queue2){
	int somma;
	Pipeline p;
	for(int i=0; i<6; i++){
		msgrcv(queue1,&p,sizeof(Pipeline)-sizeof(long),0,0);
		if(p.tipo==MSG_ERROR){
			printf("Non tutti i messaggi possono essere elaborati!\n");
			msgsnd(queue2,&p,sizeof(Pipeline)-sizeof(long),0);
			exit(0);
		} else{
			somma=0;
			int i=0;
			while(p.string[i]!='\0'){
				somma+=p.string[i];
				i++;
			}
			for(int i=0; i<2; i++){
				somma+=p.ar[i];
			}
			p.var=somma;
			msgsnd(queue2,&p,sizeof(Pipeline)-sizeof(long),0);
		}
	}
}

void Visualizzatore(int queue){
	Pipeline p;
	for(int i=0; i<6; i++){
		msgrcv(queue,&p,sizeof(Pipeline)-sizeof(long),0,0);
		if(p.tipo==MSG_ERROR){
			printf("Non tutti i messaggi possono essere elaborati!\n");
			exit(0);
		}
		printf("Il messaggio memorizzato è: <%s> e var: %d\n",p.string,p.var);
	}
}
