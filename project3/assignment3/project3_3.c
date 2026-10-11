
/*
Maris Athanasios        03045
Sialtsis Alexandros     03083

This program simulates a single lane bridge.
The bridge has a max cars capacity and the cars
on the bridge should have the same direction.
The synchronasation is done using the pthread_mutex
and pthread_cond funtions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct arguments_ {
    pthread_mutex_t mtx;
    pthread_mutex_t mtx2;
    pthread_cond_t train;
    pthread_cond_t passengers;
    pthread_cond_t full;
    int avl;
    int capacity;
} arguments;


void *train_thread(void *arg) {
    arguments *args = arg;
    pthread_mutex_t *mtx = &args->mtx;
    pthread_cond_t *train = &args->train;
    pthread_cond_t *passengers = &args->passengers;

    pthread_mutex_lock(mtx);

    while(1) {
        printf("train is going to wait\n");
        pthread_cond_wait(train, mtx);

        printf("\nthe train is full and the ride begins!!\n");
        sleep(5);
        printf("the ride ended!\n");
        
        pthread_cond_broadcast(passengers);
    }

    pthread_mutex_unlock(mtx);

    return(NULL);
}

void *passenger_thread(void *arg) {
    arguments *args = arg;
    pthread_mutex_t *mtx = &args->mtx;
    pthread_mutex_t *mtx2 = &args->mtx2;
    pthread_cond_t *train = &args->train;
    pthread_cond_t *passengers = &args->passengers;
    pthread_cond_t *full = &args->full;
    int *avl = &args->avl, capacity = args->capacity;

    pthread_mutex_lock(mtx2);
    while(*avl == 0) pthread_cond_wait(full, mtx2);

    pthread_mutex_lock(mtx);
    
    (*avl)--;
    printf("passenger on board\n");

    if(*avl == 0) {
        printf("waking up the train\n");
        pthread_cond_signal(train);
    }

    pthread_mutex_unlock(mtx2);

    
    pthread_cond_wait(passengers, mtx);
    printf("leaving the train\n");

    (*avl)++;
    if(*avl == capacity) pthread_cond_broadcast(full);

    pthread_mutex_unlock(mtx);

    return(NULL);
}

int main(int argc, char *argv[]) {
    arguments args;
    pthread_t train_thread_id, passenger_thread_id;
    int add;

    if(argc != 2) {
        printf("give capacity as argument\n");
        return(0);
    }
    
    args.capacity = atoi(argv[1]);
    args.avl = args.capacity;
    
    pthread_mutex_init(&args.mtx, NULL);
    pthread_mutex_init(&args.mtx2, NULL);
    pthread_cond_init(&args.train, NULL);
    pthread_cond_init(&args.passengers, NULL);
    pthread_cond_init(&args.full, NULL);

    pthread_create(&train_thread_id, NULL, train_thread, &args);

    while(1) {
        scanf(" %d", &add);
        if(add == -1) break;
        if(add == 1) {
            pthread_create(&passenger_thread_id, NULL, passenger_thread, &args);
        }
    }

    pthread_mutex_destroy(&args.mtx);
    pthread_mutex_destroy(&args.mtx2);
    pthread_cond_destroy(&args.train);
    pthread_cond_destroy(&args.passengers);
    pthread_cond_destroy(&args.full);

    return(0);
}