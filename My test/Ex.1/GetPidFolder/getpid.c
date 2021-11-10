#include <stdio.h>
#include <unistd.h> //Libreria per la fork

int main(void){
	int pid,ppid;
	pid = getpid();
	printf("Questo è il PID del processo: %d\n",pid);
	
	ppid = getppid();
	printf("Questo è il PID del processo genitore: %d\n",ppid);

}
