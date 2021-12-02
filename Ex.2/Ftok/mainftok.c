#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <string.h>

int main() {
	int status;
	key_t chiave = ftok(".",'a');
	int ds_shm = shmget(chiave, 100, IPC_CREAT | IPC_EXCL | 0664);
	char *p;

	if(ds_shm < 0) {
		ds_shm = shmget(chiave, 100, 0664);
		p = (char*) shmat(ds_shm, NULL, 0);	
	}else {
		p = (char*) shmat(ds_shm, NULL, 0);
		strncpy(p, "HELLO WORLD", sizeof("HELLO WORLD"));
		}

	printf("Il contenuto della SHM: %s\n", p);

}






