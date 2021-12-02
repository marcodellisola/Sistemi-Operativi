#ifndef HEADER_H
#define HEADER_H

#define NUM_CONSUMATORI 5
#define NUM_PRODUTTORI 5
#define REQUEST_TO_SEND 1
#define OK_TO_SEND 2
#define MESSAGGIO 3

typedef struct{
	long tipo;
	char mess[30];
} Messaggio;

void ReciveBloc(Messaggio *,int,int);
void SendSincr(Messaggio *,int);
void initServiceQueues();
void removeServiceQueues();

void Produttore(int queue, char* msg);
void Consumatore(int queue);

#endif
