#ifndef PROCEDURE_H
#define PROCEDURE_H

struct prodcons{
	int buffer;
	int buffer_libero;
	int buffer_pieno;
	Monitor m;
};

#define VAR_PROD 0
#define VAR_CONS 1

void produci(struct prodcons *p, int valore;
int consuma(struct prodcons *p);

#endif

