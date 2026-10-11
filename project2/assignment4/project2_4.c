
/*
Maris Athanasios        03045
Sialtsis Alexandros     03083

This program simulates a roller-coster train
and the passengers waiting to get in. The train
is one thread and each passenger is another.
The synchronasation is done using binary semaphores.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include "semaphores.h"

typedef struct arguments_ {
    mysem_t *mtx;
    mysem_t *mtx2;
    mysem_t *train;
    mysem_t *passengers;
    int *avl;
    int capacity;
} arguments;


void *train_thread(void *arg) {
    arguments *args = arg;
    mysem_t *mtx = args->mtx;
    mysem_t *train = args->train;
    mysem_t *passengers = args->passengers;
    int *avl = args->avl;
    int capacity = args->capacity;

    while(1) {
        printf("train is going to wait\n");
        mysem_down(train);

        mysem_down(mtx);

        printf("\nthe train is full and the ride begins!!\n");
        sleep(5);
        printf("the ride ended!\n");

        *avl = capacity;
        
        mysem_up(passengers);
        mysem_up(mtx);
    }

    return(NULL);
}

void *passenger_thread(void *arg) {
    arguments *args = arg;
    mysem_t *mtx = args->mtx;
    mysem_t *mtx2 = args->mtx2;
    mysem_t *train = args->train;
    mysem_t *passengers = args->passengers;
    int *avl = args->avl;

    mysem_down(mtx2);
    if(*avl == 0) {
        mysem_down(passengers);
        mysem_up(passengers);
    }

    mysem_down(mtx);
    printf("\nmtx locked\n");
    
    if(*avl > 0) {
        (*avl)--;
        printf("passenger on board\n");
    }

    if(*avl == 0) {
        printf("waking up the train\n");
        mysem_down(passengers);
        mysem_up(train);
    }
    printf("mtx unlocked\n");
    mysem_up(mtx2);
    mysem_up(mtx);

    return(NULL);
}

int main(int argc, char *argv[]) {
    mysem_t *mtx, *mtx2, *train, *passengers;
    arguments args;
    pthread_t train_thread_id, passenger_thread_id;
    int capacity, avl, add;

    if(argc != 2) {
        printf("give capacity as argument\n");
        return(0);
    }
    
    capacity = atoi(argv[1]);
    avl = capacity;

    mysem_calloc(&mtx);
    mysem_calloc(&mtx2);
    mysem_calloc(&train);
    mysem_calloc(&passengers);
    
    mysem_init(mtx, 1);
    mysem_init(mtx2, 1);
    mysem_init(train, 0);
    mysem_init(passengers, 1);

    args.mtx = mtx;
    args.mtx2 = mtx2;
    args.train = train;
    args.passengers = passengers;
    args.avl = &avl;
    args.capacity = capacity;

    pthread_create(&train_thread_id, NULL, train_thread, &args);

    while(1) {
        scanf(" %d", &add);
        if(add == -1) break;
        if(add == 1) {
            pthread_create(&passenger_thread_id, NULL, passenger_thread, &args);
        }
    }

    return(0);
}
