
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "4_1_coro.h"
#include "4_2_mythreads.h"

#define FROM_LEFT 0
#define FROM_RIGHT 1
#define NONE 2
#define MAX_CAR_THREADS 50

int car_count = 0, bridge_direction = NONE, max_cars;
int blocked_from_left = 0, blocked_from_right = 0;
thr_t *cars;

void bridge_enter(int direction) {

    while(car_count == max_cars || (direction != bridge_direction && bridge_direction != NONE)) {
        if(direction == FROM_LEFT) {
            blocked_from_left++;
            mythreads_tuple_in("%s","FROM_LEFT");
            blocked_from_left--;
        }
        else {
            blocked_from_right++;
            mythreads_tuple_in("%s","FROM_RIGHT");
            blocked_from_right--;
        }
    }

    car_count++;
    bridge_direction = direction;

}

void bridge_leave(int direction) {
    int i;

    car_count--;
    if (car_count == 0) {
        bridge_direction = NONE;
    }

    if (direction == FROM_LEFT && blocked_from_right > 0) {
        for(i = 0; i < max_cars; i++) {
            mythreads_tuple_out("%s","FROM_RIGHT");
        }
    }
    else if (direction == FROM_RIGHT && blocked_from_left > 0) {
        for(i = 0; i < max_cars; i++) {
            mythreads_tuple_out("%s","FROM_LEFT");
        }
    }
    else if (direction == FROM_LEFT && blocked_from_left > 0) {
        for (i = 0; i < max_cars; i++) {
            mythreads_tuple_out("%s","FROM_LEFT");
        }
    }
    else if (direction == FROM_RIGHT && blocked_from_right > 0) {
        for(i = 0; i < max_cars; i++) {
            mythreads_tuple_out("%s","FROM_RIGHT");
        }
    }
}

/*
void bridge_enter(int direction) {
    mythreads_tuple_in("%s", "LOCK");
    int bridge_direction;

    while(1){
        mythreads_tuple_in("%s &n","BRIDGE_DIRECTION", &bridge_direction);
        if(bridge_direction == NONE || bridge_direction == direction) {
            mythreads_tuple_out("%s %d","BRIDGE_DIRECTION", direction);
            break;
        }
        else {
            mythreads_tuple_out("%s %d","BRIDGE_DIRECTION",bridge_direction);
            //mythreads_tuple_in("%s %d", )
        }
    }
    mythreads_tuple_in("%s", "CAR");
    car_count++;
    mythreads_tuple_out("%s", "LOCK");
}


void bridge_leave(int direction) {
    mythreads_tuple_in("%s", "LOCK");
    mythreads_tuple_out("%s", "CAR");
    car_count--;
    if(car_count == 0);
    mythreads_tuple_in("%s %d", "BRIDGE_DIRECTION", direction);
    mythreads_tuple_out("%s %d", "BRIDGE_DIRECTION", NONE);
    mythreads_tuple_out("%s", "LOCK");
}
*/

void *car_thread (void *arg) {
    mythreads_start();

    int direction = *((int*) arg);
    free(arg);

    bridge_enter(direction);

    if(direction == FROM_LEFT) printf("entered the bridge from left\n...\n");
    else printf("entered the bridge from right\n...\n");

    sleep(3);

    if(direction == FROM_LEFT) printf("left the bridge (left)\n");
    else printf("left the bridge (right)\n");
    bridge_leave(direction);

    mythreads_finish();
    return(NULL);
}

int main(int argc,char *argv[]) {
    int *direction;
    char sel;
    int i = 0;
    int no_cars = 0;


    if (argc != 2) {
        printf("give max cars as argument\n");
        return(0);
    }
    max_cars = atoi(argv[1]);

    mythreads_init();

    mythreads_tuple_out("%s","LOCK");
    for (i = 0; i < max_cars; i++) {
        mythreads_tuple_out("%s","CAR");
    }
    mythreads_tuple_out("%s %d", "BRIDGE_DIRECTION", NONE);

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

        no_cars++;

        cars = realloc(cars,no_cars * sizeof(thr_t));
        if (cars == NULL) {
            printf("realloc error at %d\n",__LINE__);
        }

        mythreads_create(&cars[no_cars - 1], (void(*)())car_thread, (void*) direction);
    }
    free(cars);
    return(0);
}
