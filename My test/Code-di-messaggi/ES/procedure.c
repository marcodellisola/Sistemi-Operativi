#include <stdio.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include "header.h"

static int queue1;
static int queue2;

void initServiceQueues(){
	queue1=msgget(IPC_PRIVATE, IPC_CREAT | 0664);
	queue2=msgget(IPC_PRIVATE, IPC_CREAT | 0664);
}

void removeServiceQueues(){
	msgctl(queue1,IPC_RMID, 0);
	msgctl(queue2,IPC_RMID, 0);
}

void ReciveBloc(Messaggio * m, int queue, int tipomess){
	Messaggio m1,m2;
	msgrcv(queue1,&m1,sizeof(Messaggio)-sizeof(long),REQUEST_TO_SEND,0);
	m2.tipo=OK_TO_SEND;
	strcpy(m2.mess,"Ready to send");
	msgsnd(queue2,&m2,sizeof(Messaggio)-sizeof(long),0);
	msgrcv(queue,m,sizeof(Messaggio)-sizeof(long),tipomess,0);
}

void SendSincr(Messaggio *m, int queue){
	Messaggio m1,m2;
	m1.tipo=REQUEST_TO_SEND;
	strcpy(m1.mess,"Request to send!");
	msgsnd(queue1,&m1,sizeof(Messaggio)-sizeof(long),0);
	msgrcv(queue2,&m2,sizeof(Messaggio)-sizeof(long),OK_TO_SEND,0);
	msgsnd(queue,m,sizeof(Messaggio)-sizeof(long),0);
}

void Scrittore(int queue){
	Messaggio m;
	while(1){
		printf("Inserire messaggio da inviare: ");
		scanf("%s",m.mess);
		m.tipo=MESSAGGIO;
		if(strcmp(m.mess,"exit")==0){
			exit(1);
		} else {
                        SendSincr(&m,queue);
                        printf("Messaggio inviato: <%s>\n",m.mess);
		}
	}
}

void Lettore(int queue){
	Messaggio m;
	while(1){
		ReciveBloc(&m,queue,MESSAGGIO);
		printf("Messaggio ricevuto: <%s>\n",m.mess);
		if(strcmp(m.mess,"exit")==0) {exit(1);}
	}
}
