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

void Produttore(int queue, char* msg){
	Messaggio m;
	m.type=MESSAGGIO;
	strcpy(m.msg,msg);
	msgsnd(queue,(void*)&m,sizeof(Messaggio)-sizeof(long),IPC_NOWAIT);
	printf("Messaggio Inviato: <%s>\n",m.msg);	
}

void Consumatore(int queue){
	Messaggio m;
	msgrcv(queue,(void*)&m,sizeof(Messaggio)-sizeof(long),MESSAGGIO,0);
	printf("Messaggio Ricevuto: <%s>\n",m.msg);
}

