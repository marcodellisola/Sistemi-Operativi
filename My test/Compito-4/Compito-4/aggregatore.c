#include "sensore.h"
#include "aggregatore.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

void aggregatore(int id_coda_sensore, int id_code_collettori[3]) {

    printf("Avvio processo aggregatore...\n");


    /* TBD: Completare questa funzione, avviando un thread scrittore e 3 thread lettori *
     *
     *      Nota: occorre passare ai thread sia il puntatore ad un oggetto-monitor,
     *      sia l'identificativo della coda di messaggi su cui ricevere/inviare.
     */
   pthread_t threads[4];
   pthread_attr_t attr;
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_JOINABLE);
   MonitorLS *m = (MonitorLS*) malloc(sizeof(MonitorLS));
   pthread_mutex_init(&(m->mutex),NULL);
   pthread_cond_init(&(m->ok_scritt_cv),NULL);
   pthread_cond_init(&(m->ok_lett_cv),NULL);
   m->num_scrittori=0;
   m->num_lettori=0;

    printf("Inizio thread scrittore!\n");
    pthread_create(&threads[0],&attr,thread_scrittore((void*)m,id_coda_sensore),(void*)m);
    for(int i=1; i<3; i++){
    	printf("Inizio thread lettore %d!\n",i);
        pthread_create(&threads[i],&attr,thread_lettore((void*)m,id_code_collettori[i]),(void*)m);
    }

    pthread_join(threads[0],NULL);
   printf("Threads scrittore terminato!\n");
   for(int i=1; i<3; i++){
           pthread_join(threads[i],NULL);
           printf("Threads lettore %d terminato!\n",i);
   }

   pthread_mutex_destroy(&(m->mutex)); 
   pthread_cond_destroy(&(m->ok_scritt_cv));
   pthread_cond_destroy(&(m->ok_lett_cv));
   pthread_attr_destroy(&attr);
   free(m);

}


void * thread_lettore(void * x,int id_coda_collettore) {
 MonitorLS *m = (MonitorLS*) x;

    for(int i=0; i<10; i++) {

        int valore;

        sleep(1);

        /* TBD: Chiamare il metodo "lettura()" del monitor */
	lettura(m,valore);
	

        printf("Aggregatore: Invio valore=%d\n", valore);

        /* TBD: Inviare il messaggio */
	messaggio p;
	p.tipo=TIPO;
	p.valore=valore;
	msgsnd(id_coda_collettore,&p,sizeof(messaggio)-sizeof(long),0);
    }


    pthread_exit(NULL);
}

void * thread_scrittore(void * x,int id_coda_sensore) {
	MonitorLS *p = (MonitorLS*) x;

    for(int i=0; i<10; i++) {

        printf("Aggregatore: In attesa di messaggi...\n");

        /* TBD: Ricevere il messaggio */
	messaggio m;
	msgrcv(id_coda_sensore,&m,sizeof(messaggio)-sizeof(long),TIPO,0);

        int valore = /* TBD */m.valore;

        printf("Aggregatore: Ricevuto valore=%d\n", valore);

        /* TBD: Chiamare il metodo "scrittura()" del monitor */
	scrittura(p,valore);
    }

    pthread_exit(NULL);
}

void lettura(MonitorLS * m, int * valore) {

    /* TBD: Completare il metodo, con la sincronizzazione */
	pthread_mutex_lock(&(m->mutex));
	m->num_lettori++;
	while(m->num_scrittori>0){
		pthread_cond_wait(&(m->ok_lett_cv),&(m->mutex));
	}
    *valore = m->variabile;

    printf("Aggregatore: Lettura valore=%d\n", *valore);
    
    m->num_lettori--;
    if(m->num_lettori==0){
	    pthread_cond_signal(&(m->ok_scritt_cv));
    } else {
	    pthread_cond_signal(&(m->ok_lett_cv));
    }
    pthread_mutex_unlock(&(m->mutex));

}

void scrittura(MonitorLS * m, int valore) {
    
    /* TBD: Completare il metodo, con la sincronizzazione */
	pthread_mutex_lock(&(m->mutex));
	m->num_scrittori++;
	while(m->num_lettori>0){
    		pthread_cond_wait(&(m->ok_scritt_cv),&(m->mutex));
	}
    printf("Aggregatore: Scrittura valore=%d\n", valore);

    m->variabile = valore;

    m->num_scrittori--;
    if(m->num_scrittori>0){
    	pthread_cond_signal(&(m->ok_scritt_cv));
    } else {
	pthread_cond_signal(&(m->ok_lett_cv));
    }
    pthread_mutex_unlock(&(m->mutex));
}
