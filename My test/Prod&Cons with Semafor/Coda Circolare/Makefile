all: main

main: procedure.o sem.o main.o
	gcc -o main procedure.o sem.o main.o

main.o: main.c
	gcc -c main.c

procedure.o: procedure.h procedure.c
	gcc -c procedure.c

semafori.o: sem.c sem.h
	gcc -c sem.c



clean:
	rm -rf *.o
	rm -rf main
