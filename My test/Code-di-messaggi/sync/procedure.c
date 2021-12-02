#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "header.h"
#include <time.h>

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

void SendSincr(Messaggio *m, int queue){
	Messaggio m1,m2;
	m1.tipo=REQUEST_TO_SEND;
	strcpy(m1.mess,"Richiesta di invio!\n");
	msgsnd(queue1,&m1,sizeof(Messaggio)-sizeof(long),0);
	msgrcv(queue2,&m2,sizeof(Messaggio)-sizeof(long),OK_TO_SEND,0);
	msgsnd(queue,m,sizeof(Messaggio)-sizeof(long),0);
}

void ReceiveBloc(Messaggio *m, int queue, int tipomess){
	Messaggio m1,m2;
	msgrcv(queue1,&m1,sizeof(Messaggio)-sizeof(long),REQUEST_TO_SEND,0);
	m2.tipo=OK_TO_SEND;
	strcpy(m2.mess,"Ready to send");
	msgsnd(queue2,&m2,sizeof(Messaggio)-sizeof(long),0);
	msgrcv(queue,m,sizeof(Messaggio)-sizeof(long),tipomess,0);
}

void Produttore(int queue, char *msg){
	Messaggio m;
	m.tipo=MESSAGGIO;
	strcpy(m.mess,msg);
	SendSincr(&m,queue);
	printf("Messaggio inviato: <%s>\n",m.mess);
}

void Consumatore(int queue){
	Messaggio m;
	ReceiveBloc(&m, queue, MESSAGGIO);
	printf("Messaggio ricevuto: <%s>\n",m.mess);
}
