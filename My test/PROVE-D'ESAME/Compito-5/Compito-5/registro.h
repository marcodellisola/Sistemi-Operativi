#ifndef _REGISTRO_H_
#define _REGISTRO_H_

// Messaggi da e verso il registro
typedef struct {
    /* TBD */
	long type;
	int id;
	int identificativo;
} messaggio_registro;

// Messaggi verso il server
typedef struct {
    /* TBD */
	long type;
	int valore;
} messaggio_server;

/* TBD: Definire i tipi di messaggio */
#define BIND 1
#define QUERY 2
#define SERVICE 3
#define RESULT 4

void client(int id_coda_registro_richieste, int id_coda_registro_risposte);
void registro(int id_coda_registro, int id_coda_registro_risposte);
void server(int id_coda_registro, int id_coda_registro_risposte, int id_server);

#endif
