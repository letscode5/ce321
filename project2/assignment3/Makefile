
CC = gcc
CCFLAGS = -Wall -g -pthread

all: project2_3 clear

project2_3: semaphores.o project2_3.o
	$(CC) $(CCFLAGS) semaphores.o project2_3.o -o project2_3
project2_3.o: project2_3.c semaphores.h
	$(CC) $(CCFLAGS) -c project2_3.c
semaphores.o: semaphores.c semaphores.h
	$(CC) $(CCFLAGS) -c semaphores.c

clear:
	rm *.o

.PHONY: clean
clean:
	rm project2_3
