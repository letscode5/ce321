
/*
Maris Athanasios        03045
Sialtsis Alexandros     03083

This program simulates the single lane bridge
problem using threads. The synchronisation
is done using binary semaphores.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <errno.h>
#include "semaphores.h"

#define MAX_CARS 5

#define FROM_LEFT 0
#define FROM_RIGHT 1
#define EMPTY 2

struct arguments_ {
    mysem_t *left_light;
    mysem_t *right_light;
    mysem_t *count_mtx;
    mysem_t *block_mtx;
    mysem_t *enter_mtx;
    mysem_t *left_full;
    mysem_t *right_full;
    int direction;
};

typedef struct arguments_ arguments;


int car_count = 0;
int bridge_direction = EMPTY;
int woke_from_left = 0;
int woke_from_right = 0;

mysem_t *main_block;


int bridge_enter(arguments *args, int direction) {
    mysem_t *left_light = args->left_light;
    mysem_t *right_light = args->right_light;
    mysem_t *count_mtx = args->count_mtx;
    mysem_t *block_mtx = args->block_mtx;
    mysem_t *enter_mtx = args->enter_mtx;
    mysem_t *left_full = args->left_full;
    mysem_t *right_full = args->right_full;
    int is_full;

    // block if full
    mysem_down(count_mtx);
    is_full = (car_count == MAX_CARS) ? 1 : 0;
    mysem_up(count_mtx);
    if(is_full == 1){
        if(direction == FROM_LEFT) {
            mysem_down(left_full);
            mysem_down(block_mtx);
            woke_from_left++;
            if(woke_from_left < MAX_CARS) mysem_up(left_full);
            mysem_up(block_mtx);
        }
        else {
            mysem_down(right_full);
            mysem_down(block_mtx);
            woke_from_right++;
            if(woke_from_right < MAX_CARS) mysem_up(right_full);
            mysem_up(block_mtx);
        }
    }
    else {
        if(direction == FROM_LEFT) {
            mysem_down(block_mtx);
            woke_from_left++;
            mysem_up(block_mtx);
        }
        else {
            mysem_down(block_mtx);
            woke_from_right++;
            mysem_up(block_mtx);
        }
    }

    // block if different direction
    if(direction != bridge_direction && bridge_direction != EMPTY) {
        if(direction == FROM_LEFT) {
            mysem_down(left_light);
            mysem_up(left_light);
        }
        else {
            mysem_down(right_light);
            mysem_up(right_light);
        }
    }

    // change bridge direction and traffic light
    mysem_down(enter_mtx);
    if(car_count == 0) {
        bridge_direction = direction;
        if(direction == FROM_LEFT) mysem_down(right_light);
        else mysem_down(left_light);
    }
    mysem_down(count_mtx);  // change car count
    car_count++;
    mysem_up(count_mtx);
    mysem_up(enter_mtx);


    return(1);
}

int bridge_leave(arguments *args, int direction) {
    mysem_t *left_light = args->left_light;
    mysem_t *right_light = args->right_light;
    mysem_t *count_mtx = args->count_mtx;
    mysem_t *block_mtx = args->block_mtx;
    mysem_t *left_full = args->left_full;
    mysem_t *right_full = args->right_full;
    int is_empty;

    // change car count
    mysem_down(count_mtx);
    car_count--;
    mysem_up(count_mtx);
    mysem_down(block_mtx);
    if(direction == FROM_LEFT) woke_from_left--;
    else woke_from_right--;
    mysem_up(block_mtx);

    // unblock
    mysem_down(count_mtx);
    is_empty = (car_count == 0) ? 1 : 0;
    mysem_up(count_mtx);
    if(is_empty == 1) {
        bridge_direction = EMPTY;
        if(direction == FROM_LEFT) {
            mysem_up(left_full);
            if (woke_from_right < MAX_CARS) {
                mysem_up(right_full);
            }
            mysem_up(right_light);
        }
        else {
            if (woke_from_left < MAX_CARS) {
                mysem_up(left_full);
            }
            mysem_up(right_full);
            mysem_up(left_light);
        }
    } 


    return(1);
}

void *car_thread (void *arg) {
    arguments *args = (arguments *) arg;
    int direction = args->direction;

    mysem_up(main_block);

    bridge_enter((arguments *) arg, direction);

    printf("entered the bridge ");
    if(direction == FROM_LEFT) printf("from left\n...\n");
    else printf("from right\n...\n");

    sleep(3);

    printf("left the bridge\n");
    bridge_leave((arguments *) arg, direction);
    return(NULL);
}

int main(int argc,char *argv[]) {
    mysem_t *left_light;
    mysem_t *right_light;
    mysem_t *count_mtx;
    mysem_t *block_mtx;
    mysem_t *enter_mtx;
    mysem_t *left_full;
    mysem_t *right_full;
    char sel;

    arguments args;
    pthread_t car;

    mysem_calloc(&left_light);
    mysem_calloc(&right_light);
    mysem_calloc(&count_mtx);
    mysem_calloc(&block_mtx);
    mysem_calloc(&enter_mtx);
    mysem_calloc(&main_block);
    mysem_calloc(&left_full);
    mysem_calloc(&right_full);

    mysem_init(main_block, 1);
    mysem_init(left_light, 1);
    mysem_init(right_light, 1);
    mysem_init(count_mtx, 1);
    mysem_init(block_mtx, 1);
    mysem_init(enter_mtx, 1);
    mysem_init(left_full, 0);
    mysem_init(right_full, 0);

    args.left_light = left_light;
    args.right_light = right_light;
    args.count_mtx = count_mtx;
    args.block_mtx = block_mtx;
    args.enter_mtx = enter_mtx;
    args.left_full = left_full;
    args.right_full = right_full;

    while(1) {
        scanf(" %c", &sel);
        mysem_down(main_block);
        if(sel == 'l') {
            args.direction = FROM_LEFT;
        }
        else if(sel == 'r') {
            args.direction = FROM_RIGHT;
        }
        else if(sel == 'q') {
            break;
        }
        else {
            continue;
        }
        pthread_create(&car,NULL,car_thread,&args);
    }


    return(0);
}
