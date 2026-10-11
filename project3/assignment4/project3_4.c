
/*
Maris Athanasios        03045
Sialtsis Alexandros     03083

This program simulates a mixed WC. The WC has
certain capacity and it must be used only by
one gender at a time. Womem must be prioritized.
The synchronasation is done using the pthread_mutex
and pthread_cond funtions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAN 1
#define WOMAN 2

int capacity;
int wc_rn;
int counter;
int woman_count;
pthread_cond_t man_block;
pthread_cond_t woman_block;
pthread_mutex_t mutex;

void wc_enter(int gender) {
    pthread_mutex_lock(&mutex);
    while (counter == capacity || (gender != wc_rn && wc_rn != 0)) {
        if (gender == WOMAN) {
            woman_count++;
            pthread_cond_wait(&woman_block,&mutex);
            woman_count--;
        }
        else {
            pthread_cond_wait(&man_block,&mutex);
        }
    }
    counter++;

    wc_rn = gender;

    pthread_mutex_unlock(&mutex);
}

void wc_leave(int gender) {
    int i;
    pthread_mutex_lock(&mutex);

    counter--;

    if (counter == 0) {
        wc_rn = 0;
    }

    if (woman_count > 0) {
        for (i = 0; i < capacity; i++) {
            pthread_cond_signal(&woman_block);
        }
    }
    else {
        for (i = 0; i < capacity; i++) {
            pthread_cond_signal(&man_block);
        }
    }

    pthread_mutex_unlock(&mutex);
}

void* human_thread(void *arg) {
    int gender = *((int*) arg);

    free(arg);
    wc_enter(gender);
    if (gender == MAN) printf("Entered WC (Man)\n");
    else printf("Entered WC (Woman)\n");

    sleep(3);

    if (gender == MAN) printf("Left WC (Man)\n");
    else printf("Left WC (Woman)\n");
    wc_leave(gender);
    return(NULL);
}

int main(int argc,char *argv[]) {
    pthread_t human;
    char gender_c;
    int *gender_i;
    if (argc != 2) {
        printf("give capacity as argument\n");
        return(0);
    }

    capacity = atoi(argv[1]);

    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&woman_block,NULL);
    pthread_cond_init(&man_block,NULL);

    while(1) {
        scanf(" %c",&gender_c);
        gender_i = malloc(sizeof(int*));
        if (gender_c == 'm') {
            *gender_i = MAN;
        }
        else if (gender_c == 'w') {
            *gender_i = WOMAN;
        }
        else if (gender_c == 'q') break;
        else continue;
        pthread_create(&human,NULL,human_thread,(void*) gender_i);
    }
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&woman_block);
    pthread_cond_destroy(&man_block);
    return(0);
}
