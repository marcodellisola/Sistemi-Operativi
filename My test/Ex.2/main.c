#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
	key_t chiave = ftok(".",'a');
	int ds_shm = shmget(chiave, 100, IPC_CREAT | IPC_EXCL | 0664);
	char *p;

	if(ds_shm < 0){
		ds_shm = shmget(chiave,100,0664);
		p = (char*) shmat(ds_shm, NULL, 0);
	}else{
		p = (char*) shmat(ds_shm, NULL,0);
	}

	if(strcmp(argv[1],"w")==0){
		strncpy(p,argv[2],sizeof(argv[2]));
	}

	if(strcmp(argv[1],"r")==0){
		ds_shm = shmctl(chiave, IPC_RMID,);
		printf("Shared Memory eliminata!");
	}

	if(strcmp(argv[1],"d")==0){
		printf("Il contenuto della SHM è: %s\n",p);
	}
	
}
