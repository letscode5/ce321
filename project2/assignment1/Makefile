
CC = gcc
CCFLAGS = -Wall -g -pthread

all: semaphores clear

semaphores: semaphores.o
	$(CC) $(CCFLAGS) semaphores.o -o semaphores
semaphores.o: semaphores.c semaphores.h
	$(CC) $(CCFLAGS) -c semaphores.c

clear:
	rm *.o

.PHONY: clean
clean:
	rm semaphores
