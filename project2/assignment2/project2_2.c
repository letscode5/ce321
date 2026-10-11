
/*
Maris Athanasios        03045
Sialtsis Alexandros     03083

This program takes a number of integers and
checks if they are prime numbers using threads. The
synchronisation is done using our binary semaphores.
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
#include"semaphores.h"

struct arguments {
    int num;
    mysem_t *s;
};

void *prime(void *arg) {
    int i, n, ans;
    struct arguments *tmp = (struct arguments *) arg;
    int *number = &tmp->num;
    mysem_t *s =  tmp->s;
    
    while(1) {
        mysem_down(s);
        n = *number;
        ans = 1;
        for(i = 2; i <= n / 2; i++) {
            if(n % i == 0) {
                ans = 0;
                break;
            }
        }
        printf("%d : %d\n", n, ans);
    }
    return(NULL);
}

int main(int argc, char *argv[]) {
    int i, workers, num;
    pthread_t *threads;
    struct arguments *args;

    if(argc != 2) return(0);
    workers = atoi(argv[1]);
    threads = (pthread_t *) calloc(sizeof(pthread_t),workers);
    args = (struct arguments *) calloc(sizeof(struct arguments),workers);
    
    for(i = 0; i < workers; i++) {
        mysem_calloc(&args[i].s);
        mysem_init(args[i].s, 0);
        pthread_create(&threads[i], NULL, prime, &args[i]);
    }

    i = 0;
    while(1) {
        scanf(" %d", &num);
        if(num == 0) break;
        if(num < 0) continue;
        while(semctl(args[i].s->semid, 0, GETVAL) == 0 && semctl(args[i].s->semid, 0, GETNCNT) == 0) {
            i = (i == workers - 1) ? 0 : i + 1;
        }
        args[i].num = num;
        mysem_up(args[i].s);
        i = (i == workers - 1) ? 0 : i + 1; 
    }
    
    for(i = 0; i < workers; i++) {
        if(semctl(args[i].s->semid, 0, GETVAL) == 0 && semctl(args[i].s->semid, 0, GETNCNT) == 0) {
            i--;
        }
    }

    free(threads);
    free(args);
    return(0);
}
