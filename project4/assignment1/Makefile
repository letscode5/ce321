
CC = gcc
CCFLAGS = -Wall -g

all: 4_1 clear

4_1: 4_1_coro.o 4_1_pipe.o 4_1_pipe_coro.o 4_1_pipe_main.o 4_1_mydiff.o
	$(CC) $(CCFLAGS) 4_1_coro.o 4_1_pipe.o 4_1_pipe_coro.o 4_1_mydiff.o 4_1_pipe_main.o -o 4_1
4_1_pipe_main.o: 4_1_pipe_main.c 4_1_pipe.h 4_1_coro.h
	$(CC) $(CCFLAGS) -c 4_1_pipe_main.c 
4_1_pipe_coro.o: 4_1_pipe_coro.c 4_1_pipe.h 4_1_coro.h
	$(CC) $(CCFLAGS) -c 4_1_pipe_coro.c 
4_1_mydiff.o: 4_1_mydiff.c 4_1_pipe.h 4_1_coro.h
	$(CC) $(CCFLAGS) -c 4_1_mydiff.c
4_1_pipe.o: 4_1_pipe.c 4_1_pipe.h 4_1_coro.h
	$(CC) $(CCFLAGS) -c 4_1_pipe.c 
4_1_coro.o: 4_1_coro.c 4_1_coro.h
	$(CC) $(CCFLAGS) -c 4_1_coro.c 

clear:
	rm *.o

.PHONY: clean
clean:
	rm 4_1
