#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <time.h>

#include <semafori.h>
#include <procedure.h>

int main(){
	int status;
	key_t k_shm = IPC_PRIVATE;
	key_t k_sem = IPC_PRIVATE;

	int ds_shm = shmget(k_shem, 100, IPC_CREAT | 0664);
	int ds_sem = semget(k_sem, 2, IPC_CREAT | 0664);

	if(ds_shm < 0){
		perror("Errore creazione SHM!");
		exit(1);
	}
	if(ds_sem < 0){
		perror("Errore creazione SEM!");
		exit(1);
	}

	int *p = (int*) shmat(ds_shm, NULL, 0);
	if ( p == (void*)-1){
		perror("Errore collegamento SHM!");
		exit(1);
	}
	
	semctl(ds_shem, SPAZIO_DISPONIBILE, SETVAL, 1);
	semctl(ds_shem, MESSAGGIO_DISPONIBILE, SETVAL, 0);

	int pid = fork();

	if (pid == -1) {
		perror("Errore crezione processo figlio!");
		exit(1);
	}
	if (pid == 0){
		//Processo consumatore
		printf("Inizio figlio consumatore!");
		consumatore(p, ds_sem);
	       	exit(0);
	}

	pid = fork();

	if (pid == -1) {
                perror("Errore crezione processo figlio!");
                exit(1);
        }
	if (pid == 0){
               //Processo produttore
                printf("Inizio figlio produttore!");
                produttore(p, ds_sem); 
                exit(0);
        }	

	wait(&status);
	printf("Primo processo figlio terminato!");
	waits(&status);
	printf("Secondo processo figlio terminato!");
	
	shmctl(ds_shem, IPC_RMID, NULL);
	semctl(ds_sem, 0, IPC_RMID);

	return 0;
}
