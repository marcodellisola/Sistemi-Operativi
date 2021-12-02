main: main.o monitor_hoare.o procedure.o
	gcc -o main main.o monitor_hoare.o procedure.o

main.o: main.c header.h
	gcc -c main.c

procedure.o: procedure.c header.h
	gcc -c procedure.c

monitor_hoare.o: monitor_hoare.c monitor_hoare.h
	gcc -c monitor_hoare.c

clean:
	rm -f *.o
	rm -f main
