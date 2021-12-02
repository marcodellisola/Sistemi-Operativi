#ifndef HEADER_H
#define HEADER_H

#define MUTEXL 0
#define SYNCH 1

#define PATH_SHM "."
#define CHAR_SHM 'a'
#define PATH_SEM "."
#define CHAR_SEM 'b'

#define NMAX 15

typedef struct {
	int N;
        int numlettori;
        char str[NMAX];
    
} Buffer;


void Generatore(int,Buffer*);
void Elaboratore(int,Buffer*);
void Analizzatore(int, Buffer*);

#endif
