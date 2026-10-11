
/*
Maris Athanasios        03045
Sialtsis Alexandros     03083

This program simulates a roller-coster train
and the passengers waiting to get in. The train
is one thread and each passenger is another.
The synchronasation is done using the pthread_mutex
and pthread_cond funtions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define FROM_LEFT 0
#define FROM_RIGHT 1
#define NONE 2

int car_count = 0, bridge_direction = NONE, max_cars;
int blocked_from_left = 0, blocked_from_right = 0;
pthread_mutex_t mutex;
pthread_cond_t block_left;
pthread_cond_t block_right;


void bridge_enter(int direction) {
    pthread_mutex_lock(&mutex);

    while(car_count == max_cars || (direction != bridge_direction && bridge_direction != NONE)) {
        if(direction == FROM_LEFT) {
            blocked_from_left++;
            pthread_cond_wait(&block_left, &mutex);
            blocked_from_left--;
        }
        else {
            blocked_from_right++;
            pthread_cond_wait(&block_right, &mutex);
            blocked_from_right--;
        }
    }

    car_count++;
    bridge_direction = direction;

    pthread_mutex_unlock(&mutex);
}

void bridge_leave(int direction) {
    int i;

    pthread_mutex_lock(&mutex);
    car_count--;
    if (car_count == 0) {
        bridge_direction = NONE;
    }

    if (direction == FROM_LEFT && blocked_from_right > 0) {
        for(i = 0; i < max_cars; i++) {
            pthread_cond_signal(&block_right);
        }
    }
    else if (direction == FROM_RIGHT && blocked_from_left > 0) {
        for(i = 0; i < max_cars; i++) {
            pthread_cond_signal(&block_left);
        }
    }
    else if (direction == FROM_LEFT && blocked_from_left > 0) {
        for (i = 0; i < max_cars; i++) {
            pthread_cond_signal(&block_left);
        }
    }
    else if (direction == FROM_RIGHT && blocked_from_right > 0) {
        for(i = 0; i < max_cars; i++) {
            pthread_cond_signal(&block_right);
        }
    }

    pthread_mutex_unlock(&mutex);
}

void *car_thread (void *arg) {
    int direction = *((int*) arg);

    free(arg);
    bridge_enter(direction);

    if(direction == FROM_LEFT) printf("entered the bridge from left\n...\n");
    else printf("entered the bridge from right\n...\n");

    sleep(3);

    if(direction == FROM_LEFT) printf("left the bridge (left)\n");
    else printf("left the bridge (right)\n");
    bridge_leave(direction);
    return(NULL);
}

int main(int argc,char *argv[]) {
    int *direction;
    char sel;
    pthread_t car;

    if (argc != 2) {
        printf("give max cars as argument\n");
        return(0);
    }
    max_cars = atoi(argv[1]);

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&block_left,NULL);
    pthread_cond_init(&block_right,NULL);


    while(1) {
        scanf(" %c", &sel);
        direction = malloc(sizeof(int*));
        if(sel == 'l') {
            *direction = FROM_LEFT;
        }
        else if(sel == 'r') {
            *direction = FROM_RIGHT;
        }
        else if(sel == 'q') {
            break;
        }
        else {
            continue;
        }
        pthread_create(&car,NULL,car_thread,(void*) direction);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&block_left);
    pthread_cond_destroy(&block_right);

    return(0);
}
