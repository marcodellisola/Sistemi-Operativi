#ifndef HEADER_H
#define HEADER_H

#define MESSAGGIO 1
#define MSG_ERROR 2

typedef struct {
	long tipo;
	char string[10];
	int ar[2];
	int var;
} Pipeline;

void Generatore(int queue);
void Filtro(int queue1, int queue2);
void Checksum(int queue1, int queue2);
void Visualizzatore(int queue);

#endif
