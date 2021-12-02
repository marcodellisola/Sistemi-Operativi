all: main analizzatore generatore elaboratore

main: procedure.o semafori.o main.o
	gcc -o main procedure.o semafori.o main.o
main.o: main.c header.h
	gcc -c main.c

analizzatore: procedure.o semafori.o analizzatore.o
	gcc -o analizzatore procedure.o semafori.o analizzatore.o
analizzatore.o: analizzatore.c header.h
	gcc -c analizzatore.c

elaboratore: procedure.o semafori.o elaboratore.o
	gcc -o elaboratore procedure.o semafori.o generatore.o
elaboratore.o: elaboratore.c header.h
	gcc -c elaboratore.c

generatore: procedure.o semafori.o generatore.o
	gcc -o generatore procedure.o semafori.o generatore.o

generatore.o: generatore.c header.h
	gcc -c generatore.c 

procedure.o: header.h procedure.c
	gcc -c procedure.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c



clean:
	rm -rf *.o
	rm -rf main analizzatore elaboratore generatore
