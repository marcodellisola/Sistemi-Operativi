#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "prodcons.h"


void * produttore_1(void * p)
{
	MonitorPC * m = (MonitorPC*) p;

	for(int i=0; i<4; i++) {

		int valore = rand() % 10;

		/* TBD: chiamare il metodo di produzione tipo 1 */
		produci_tipo_1(m,valore);

		sleep(1);
	}

	pthread_exit(NULL);
}


void * produttore_2(void * p)
{
	MonitorPC * m = (MonitorPC*) p;
    
	for(int i=0; i<4; i++) {

		int valore = rand() % 10;

		/* TBD: chiamare il metodo di produzione tipo 2 */
		produci_tipo_2(m,valore);

		sleep(1);
	}

	pthread_exit(NULL);
}


void * consumatore_1(void * p)
{
	MonitorPC * m = (MonitorPC*) p;

	for(int i=0; i<8; i++) {

		int valore;

		/* TBD: chiamare il metodo di consumazione tipo 1 */
		consuma_tipo_1(m,valore);

		sleep(1);
	}

	pthread_exit(NULL);
}


void * consumatore_2(void * p)
{
	MonitorPC * m = (MonitorPC*) p;

	for(int i=0; i<8; i++) {

		int valore;

		/* TBD: chiamare il metodo di consumazione tipo 1 */
		consuma_tipo_2(m,valore);

		sleep(1);
	}

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{

	pthread_t thread_produttori_1[2];
	pthread_t thread_produttori_2[2];
	pthread_t thread_consumatore_1;
	pthread_t thread_consumatore_2;

	pthread_attr_t attr;
    
	int rc;
	int i;

	srand(time(NULL));

    
	/* TBD: creare e inizializzare l'oggetto monitor */
   	 MonitorPC *pc = (MonitorPC*) malloc(sizeof(MonitorPC));
	inizializza(pc);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);


	for(i=0; i<2; i++) {

		/* TBD: avviare 2 produttori di tipo 1 */
		printf("Inizio thread produttori 1!\n");
		pthread_create(&thread_produttori_1[i],&attr,produttore_1,(void*)pc);
	}

	for(i=0; i<2; i++) {

		/* TBD: avviare 2 produttori di tipo 2 */
		pthread_create(&thread_produttori_2[i],&attr,produttore_2,(void*)pc);
	}

	/* TBD: avviare consumatore di tipo 1 */
	pthread_create(&thread_consumatore_1,&attr,consumatore_1,(void*)pc);

	/* TBD: avviare consumatore di tipo 2 */
	pthread_create(&thread_consumatore_2,&attr,consumatore_2,(void*)pc);
	
	for(i=0; i<2; i++) {

		/* TBD: attendere terminazione produttori di tipo 1 */
		pthread_join(thread_produttori_1[i],NULL);
		printf("Thread produttore 1 n:%d terminato!\n");
	}


	for(i=0; i<2; i++) {

		/* TBD: attendere terminazione produttori di tipo 2 */
		pthread_join(thread_produttori_2[i],NULL);
		printf("Thread produttore 2 n:%d terminato!\n");
	}

	/* TBD: attendere terminazione consumatori di tipo 1 */
	pthread_join(thread_consumatore_1,NULL);
	printf("Thread consumatore 1 terminato!\n!");

	/* TBD: attendere terminazione consumatori di tipo 2 */
	pthread_join(thread_consumatore_2,NULL);
	printf("Thread consumatore 2 terminato!\n");

	/* TBD: rimuovere l'oggetto monitor */
	rimuovi(pc);
	free(pc);

	return 0;
}
