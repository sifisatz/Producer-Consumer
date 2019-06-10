#Sifis - Producer-Consumer - Makefile - csd3094

main.o: main.c main.h
	gcc -c main.c
all: main.o
	 gcc main.c -pthread -o main 
clean:
	rm *.o