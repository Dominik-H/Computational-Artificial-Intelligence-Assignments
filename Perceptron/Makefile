CC = cc
LIBS = -lm
FLAGS = -std=c11
OUTPUT = perceptron

all: perceptron.o
	$(CC) $(FLAGS) main.c $(LIBS) perceptron.o -o $(OUTPUT)

perceptron.o: perceptron.h perceptron.c
	$(CC) $(FLAGS) -c perceptron.c $(LIBS) -o perceptron.o

clear:
	rm -rf *.o *.exe