all: main scritt lett1 lett2

main: procedure.o semafori.o main.o
	gcc -o main procedure.o semafori.o main.o
main.o: main.c header.h
	gcc -c main.c

scritt: procedure.o semafori.o scritt.o
	gcc -o scritt procedure.o semafori.o scritt.o
scritt.o: scritt.c header.h
	gcc -c scritt.c

lett2: procedure.o semafori.o lett2.o
	gcc -o lett2 procedure.o semafori.o lett1.o
lett2.o: lett2.c header.h
	gcc -c lett2.c

lett1: procedure.o semafori.o lett1.o
	gcc -o lett1 procedure.o semafori.o lett1.o

lett1.o: lett1.c header.h
	gcc -c lett1.c 

procedure.o: header.h procedure.c
	gcc -c procedure.c

semafori.o: semafori.c semafori.h
	gcc -c semafori.c



clean:
	rm -rf *.o
	rm -rf main scritt lett2 lett1
