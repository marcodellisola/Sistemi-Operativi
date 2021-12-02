#include "procedure.h"

#include <stdio.h>

void produci(struct prodcons *p, int valore){
	enter_monitor(&(p->m));
	printf("Ingresso monitor!");
	if(p->buffer_pieno==1){
		printf("Sospensione - produzione!");
		wait_condition(&(p->m),VAR_PROD);
		printf("Ripresa - produzione!");
	}
	p->buffer = valore;
	p->buffer_libero=0;
	p->buffer_pieno=1;
	signal_condition(&(p->m),VAR_CONS);
	leave_monitor(&(p->m));
	printf("Uscita monitor!");
}

int consuma(struct prodcons *p){
	enter_monitor(&(p->m));
	printf("Ingresso monitor!");
	if(p->buffer_libero==1){
		printf("Sospensione - consumatore!");
		wait_condition(&(p->m),VAR_CONS);
		printf("Ripresa - consumatore!");
	}
	int valore = p->buffer;
	p->buffer_libeto=1;
	p->buffer_pieno=0;
	signal_condition(&(p->m),VAR_PROD);
	leave_monitor(&(p->m));
	printf("Uscita monitor!");
}
