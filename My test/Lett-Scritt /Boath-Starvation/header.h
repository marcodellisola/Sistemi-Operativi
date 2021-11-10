#ifndef HEADER_H
#define HEADER_H

#define MUTEX 0
#define MUTEXL 1
#define MUTEXS 2
#define SYNCH 3

typedef long msg;

typedef struct{
	int numlettori;
	int numscrittori;
	msg messaggio;
} Buffer;

void lettore(int, Buffer*);
void scrittore(int, Buffer*);
