
CC = gcc
CCFLAGS = -Wall -g -pthread

all: project2_4 clear

project2_4: semaphores.o project2_4.o
	$(CC) $(CCFLAGS) semaphores.o project2_4.o -o project2_4
project2_4.o: project2_4.c semaphores.h
	$(CC) $(CCFLAGS) -c project2_4.c
semaphores.o: semaphores.c semaphores.h
	$(CC) $(CCFLAGS) -c semaphores.c

clear:
	rm *.o

.PHONY: clean
clean:
	rm project2_4
