#ifndef HEADER_H
#define HEADER_H
#include "monitor_hoare.h"

#define NUM_UTENTI 10
#define VARCOND_LETTORI 0
#define VARCOND_SCRITTORI 1

typedef struct{
	int temperatura;
	unsigned int umidita;
	unsigned short pioggia;
	int buffer_lettori;
	Monitor m;
} meteo;

void Lettore(meteo *p);
void Scrittore(meteo *p);

#endif
