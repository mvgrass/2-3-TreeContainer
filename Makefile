CC = g++

CFLAGS = -c -std=c++11 -g3

all: main.o
	$(CC) -o main.out  main.o

main.o: main.cpp
	$(CC) $(CFLAGS) -o main.o  main.cpp

clean:
	rm -rf *.o all

