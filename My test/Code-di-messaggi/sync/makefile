main: procedure.o main.o
	gcc -o main procedure.o main.o

main.o: main.c header.h
	gcc -c main.c

procedure.o: procedure.c header.h
	gcc -c procedure.c

clena:
	rm -f *.o
	rm -f main
