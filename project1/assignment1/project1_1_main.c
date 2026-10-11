
/*
Maris Athanasios        03045
Sialtsis Alexandros     03083

This program implements a pipe which one thread can send
integers to another. The pipe uses a ring buffer. Also,
this program creates 2 threads which exchange data in
order to check if it's working right.

The file to be transefered must be defined at the header file.

project1_1_main.c also needs:
project1_1_pipe.c
*/

#include <stdio.h>
#include <pthread.h>
#include "project1_1_pipe.h"

int main(int argc,char *argv[]) {
    int pipe_id1,pipe_id2;
    pthread_t p1;
    pthread_t p2;

    arguments_t args;


    pipe_id1 = pipe_open(PIPE_SIZE);


    pipe_id2 = pipe_open(PIPE_SIZE);

    args.pipe_id1 = pipe_id1;
    args.pipe_id2 = pipe_id2;


    pthread_create(&p1,NULL,thread_1,&args);

    pthread_create(&p2,NULL,thread_2,&args);

    while (1) {
        if ((thread_check[0] == 1) && (thread_check[1] == 1)) {
            return(0);
        }
    }
}