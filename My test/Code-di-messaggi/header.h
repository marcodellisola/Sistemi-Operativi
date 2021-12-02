#ifndef HEADER_H
#define HEADER_H

#define NUM_CONSUMATORI 5
#define NUM_PRODUTTORI 5
#define MESSAGGIO 1

typedef struct{
	long type;
	char msg[30];
} Messaggio;

void Produttore(int queue, char* msg);
void Consumatore(int queue);

#endif
