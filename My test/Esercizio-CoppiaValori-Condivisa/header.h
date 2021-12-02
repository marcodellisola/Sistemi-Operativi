#ifndef HEADER_H
#define HEADER_H

#define MUTEXL 0
#define MUTEXS 1
#define SYNCH 2
#define MUTEX 3

typedef struct{
	int numlettori;
	int numscrittori;
	int val1;
	int val2;
} Buffer;

void lettore(int,Buffer*,int);
void scrittore(int,Buffer*);

int init_sem(){
	key_t k_sem = 41;
	int ds_sem = semget(k_sem, 4, IPC_CREAT | IPC_EXCL | 0664);
	if(ds_sem<0){
		perror("Errore SEM!");
		exit(1);
	}
	semctl(ds_sem, MUTEXL, SETVAL, 1);
        semctl(ds_sem, MUTEXS, SETVAL, 1);
        semctl(ds_sem, MUTEX, SETVAL, 1);
        semctl(ds_sem, SYNCH, SETVAL, 1);
	return ds_sem;
};

Buffer* init_shm(){
	key_t k_shm = 40;
        int ds_shm = shmget(k_shm, sizeof(Buffer), IPC_CREAT | 0664);
        if(ds_shm<0){perror("Errore SHM!"); exit(1);}
        Buffer *buf;
        buf = (Buffer*) shmat(ds_shm, NULL, 0);
        if(buf == (void*)-1){perror("Errore Attachment!"); exit(1);}

        buf->numlettori = 0;
	buf->numscrittori = 0;
	return buf;
};

#endif
