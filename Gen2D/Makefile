CC = gcc
LIBS = -lm
OUTPUT = gen2d

all: gen2d.o
	$(CC) -Ofast -std=c11 main.c $(LIBS) gen2d.o -o $(OUTPUT)

gen2d.o: gen2d.h gen2d.c
	$(CC) -Ofast -w -std=c11 -c gen2d.c $(LIBS) -o gen2d.o

clear:
	rm -rf *.o *.exe