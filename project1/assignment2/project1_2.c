
/*
Maris Athanasios        03045
Sialtsis Alexandros     03083

This program takes a number of integers and checks
if they are prime numbers using threads.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/*
status = 0  ---> terminated
status = 1  ---> available
status = 2  ---> job assigned
*/

struct arguments {
    int num;
    int status;
};

void *prime(void *arg) {
    int i, n, ans;
    struct arguments *tmp = (struct arguments *) arg;
    int *number = &tmp->num;
    int *status = &tmp->status;

    while(*status != 0) {
        while(*status == 2) {
            n = *number;
            ans = 1;
            for(i = 2; i <= n / 2; i++) {
                if(n % i == 0) {
                    ans = 0;
                    break;
                }
            }
            printf("%d : %d\n", n, ans);
            *status = 1;
        }
    }
    return(NULL);
}

int main(int argc, char *argv[]) {
    int i, workers, num;
    pthread_t *threads;
    struct arguments *args;

    if(argc != 2) return(0);
    workers = atoi(argv[1]);
    threads = (pthread_t *) malloc(sizeof(pthread_t) * workers);
    args = (struct arguments *) malloc(sizeof(struct arguments) * workers);
    
    for(i = 0; i < workers; i++) {
        args[i].status = 1;
        pthread_create(&threads[i], NULL, prime, &args[i]);
    }

    i = 0;
    while(1) {
        scanf(" %d", &num);
        if(num == 0) break;
        if(num < 0) continue;
        while(args[i].status != 1) {
            if(i == workers - 1) {
                i = 0;
            }
            else {
                i++;
            }
        }
        args[i].num = num;
        args[i].status = 2;
    }

    for(i = 0; i < workers; i++) {
        if(args[i].status != 1) {
            i--;
        }
    }

    free(threads);
    free(args);
    return(0);
}