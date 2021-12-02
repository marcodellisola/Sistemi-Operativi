#ifndef HEADER_H
#define HEADER_H

#define FTOK_PATH "."

#define MESSAGGIO 1
#define REQUEST_TO_SEND 2
#define OK_TO_SEND 3

typedef struct{
	long tipo;
	char mess[100];
} Messaggio;

void ReciveBloc(Messaggio *,int,int);
void SendSincr(Messaggio *,int);
void initServiceQueues();
void removeServiceQueues();

void Scrittore(int queue);
void Lettore(int queue);

#endif
