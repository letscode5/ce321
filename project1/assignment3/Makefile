
CC = gcc
CCFLAGS = -Wall -g 

all: project1_3 intstofile filetoints

project1_3: project1_3.c
	$(CC) $(CCFLAGS) -pthread project1_3.c -o project1_3

intstofile: intstofile.c
	$(CC) $(CCFLAGS) intstofile.c -o intstofile

filetoints: filetoints.c
	$(CC) $(CCFLAGS) filetoints.c -o filetoints

.PHONY: clean
clean:
	rm project1_3
