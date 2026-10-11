
CC = gcc
CCFLAGS = -Wall -g -pthread

all: project1_1 clear

project1_1: project1_1_pipe.o project1_1_threads.o project1_1_main.o
	$(CC) $(CCFLAGS) project1_1_pipe.o project1_1_threads.o project1_1_main.o -o project1_1
project1_1_main.o: project1_1_pipe.c project1_1_pipe.h
	$(CC) $(CCFLAGS) -c project1_1_main.c
project1_1_pipe.o: project1_1_pipe.c project1_1_pipe.h
	$(CC) $(CCFLAGS) -c project1_1_pipe.c 
project1_1_threads.o: project1_1_threads.c project1_1_pipe.h
	$(CC) $(CCFLAGS) -c project1_1_threads.c

clear:
	rm *.o

.PHONY: clean
clean:
	rm project1_1
