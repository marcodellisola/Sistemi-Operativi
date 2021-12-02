#include <stdio.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "header.h"

int main(int argc, char *argv[]){
	int sem = init(
	printf("Inizio lettore 1!\n");
	lettore(init_sem(),init_shm(),1);
	return 0;
}
