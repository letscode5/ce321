
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
project1_1_threads.c
project1_1_pipe.h
*/

#include <stdio.h>
#include <pthread.h>
#include "4_1_pipe.h"
#include "4_1_coro.h"

co_t main_co;
co_t co1;
co_t co2;
int pipe1;
int pipe2;

int main(int argc,char *argv[]) {
    int pipe_id1,pipe_id2;

    mycoroutines_init(&main_co);

    arguments_t args;


    pipe_id1 = pipe_open(PIPE_SIZE);
    pipe_id2 = pipe_open(PIPE_SIZE);

    args.pipe_id1 = pipe_id1;
    args.pipe_id2 = pipe_id2;
    pipe1 = pipe_id1;
    pipe2 = pipe_id2;

    mycoroutines_create(&co1, (void (*)()) coroutine1, (void *) &args);
    mycoroutines_create(&co2, (void (*)()) coroutine2, (void *) &args);

    mycoroutines_switchto(&co1);

    mydiff_all_files();

    mycoroutines_destroy(&co1);
    mycoroutines_destroy(&co2);
    return(0);
}