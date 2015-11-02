CC = cc
LIBS = -lm
FLAGS = -std=c11
OUTPUT = perceptron

all: perceptron.o
	$(CC) -Ofast $(FLAGS) main.c $(LIBS) perceptron.o -o $(OUTPUT)

perceptron.o: perceptron.h perceptron.c
	$(CC) -Ofast $(FLAGS) -c perceptron.c $(LIBS) -o perceptron.o

run:
	$(CC) -O0 $(FLAGS) -c perceptron.c $(LIBS) -o perceptron.o
	$(CC) -O0 $(FLAGS) main.c $(LIBS) perceptron.o -o $(OUTPUT)
	time ./perceptron -t "../Data/inG.dat" -w "w.dat"
	$(CC) -O1 $(FLAGS) -c perceptron.c $(LIBS) -o perceptron.o
	$(CC) -O1 $(FLAGS) main.c $(LIBS) perceptron.o -o $(OUTPUT)
	time ./perceptron -t "../Data/inG.dat" -w "w.dat"
	$(CC) -O2 $(FLAGS) -c perceptron.c $(LIBS) -o perceptron.o
	$(CC) -O2 $(FLAGS) main.c $(LIBS) perceptron.o -o $(OUTPUT)
	time ./perceptron -t "../Data/inG.dat" -w "w.dat"
	$(CC) -O3 $(FLAGS) -c perceptron.c $(LIBS) -o perceptron.o
	$(CC) -O3 $(FLAGS) main.c $(LIBS) perceptron.o -o $(OUTPUT)
	time ./perceptron -t "../Data/inG.dat" -w "w.dat"
	$(CC) -Ofast $(FLAGS) -c perceptron.c $(LIBS) -o perceptron.o
	$(CC) -Ofast $(FLAGS) main.c $(LIBS) perceptron.o -o $(OUTPUT)
	time ./perceptron -t "../Data/inG.dat" -w "w.dat"
	$(CC) -Os $(FLAGS) -c perceptron.c $(LIBS) -o perceptron.o
	$(CC) -Os $(FLAGS) main.c $(LIBS) perceptron.o -o $(OUTPUT)
	time ./perceptron -t "../Data/inG.dat" -w "w.dat"

clear:
	rm -rf *.o *.exe