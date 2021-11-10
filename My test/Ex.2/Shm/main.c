#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

int main(int argc, char *argv[]){
	key_t chiave = 40;
	int ds_shm = shmget(chiave, 100, IPC_CREAT | IPC_EXCL | 0664);
	char* p;
	struct shmid_ds buf;

	if (ds_shm < 0){
		ds_shm = shmget(chiave, 100, 0664);
		p = (char*) shmat(ds_shm, NULL, 0);
	 } else {
		 p = (char*) shmat(ds_shm, NULL, 0);
	 }

	if(p==(void*)-1){
		perror("Errore collegamento SHM!");
		exit(1);
	}

	if(strcmp(argv[1],"w")==0){
		strncpy(p, argv[2], sizeof(argv[2]));
		exit(0);
	}
	if(strcmp(argv[1],"r")==0){
		printf("Il contenuto della SHM è: %s\n",p);
		exit(0);
	}
	if(strcmp(argv[1],"d")==0){
		int d = shmctl(ds_shm, IPC_RMID, &buf);
	       	exit(0);
	}	



}
