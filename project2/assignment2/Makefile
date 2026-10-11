
CC = gcc
CCFLAGS = -Wall -g -pthread

all: project2_2 clear

project2_2: semaphores.o project2_2.o
	$(CC) $(CCFLAGS) semaphores.o project2_2.o -o project2_2
project2_2.o: project2_2.c semaphores.h
	$(CC) $(CCFLAGS) -c project2_2.c
semaphores.o: semaphores.c semaphores.h
	$(CC) $(CCFLAGS) -c semaphores.c

clear:
	rm *.o

.PHONY: clean
clean:
	rm project2_2
