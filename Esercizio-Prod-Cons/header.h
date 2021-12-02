#define SPAZIO_DISPONIBILE 0
#define MESSAGGIO_DISPONIBILE 1
#define MUTEXS 2
#define MUTEX 3

#define DIM_BUFFER 80

#define NUM_PRODUTTORI 50
#define NUM_CONSUMATORI 1

#define BUFFER_VUOTO 0
#define BUFFER_INUSO 1
#define BUFFER_PIENO 2


typedef struct{
    int id_cliente;
    int stato;
}Cliente;

void prenotazione(int,Cliente*);
void lettura(int,Cliente*);
