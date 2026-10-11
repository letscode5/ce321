
/*
Maris Athanasios        03045
Sialtsis Alexandros     03083

This program takes a number of integers and
checks if they are prime numbers using threads.
Each worker is a thread and main gives the jobs.
The synchronasation is done using the pthread_mutex
and pthread_cond funtions.
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define AVAILABLE 1
#define NOT_AVAILABLE 0

/*
status 1->available
status 0->busy
*/

int counter_threads;
pthread_mutex_t count_mtx = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t block_main = PTHREAD_COND_INITIALIZER;

struct arguments {
    int num;
    pthread_cond_t x;
    pthread_mutex_t status_mutex, tmp_mtx;
    int status;
};

void *prime(void *arg) {
    int i, n, ans;
    struct arguments *tmp = (struct arguments *) arg;
    int *number = &tmp->num, *status = &tmp->status;
    pthread_cond_t *x = &tmp->x;
    pthread_mutex_t *status_mutex = &tmp->status_mutex, *tmp_mtx = &tmp->tmp_mtx;
    
    pthread_mutex_lock(tmp_mtx);

    while(1) {
        pthread_mutex_lock(status_mutex);
        *status = AVAILABLE;
        pthread_mutex_unlock(status_mutex);
        pthread_cond_wait(x, tmp_mtx);

        n = *number;
        if(n == 0) break;
        ans = 1;
        for(i = 2; i <= n / 2; i++) {
            if(n % i == 0) {
                ans = 0;
                break;
            }
        }
        printf("                Thread %ld: %d : %d\n", pthread_self() % 1000, n, ans);
    }
    pthread_mutex_unlock(tmp_mtx);
    
    pthread_mutex_lock(&count_mtx);
    counter_threads--;
    if(counter_threads == 0) pthread_cond_signal(&block_main);
    pthread_mutex_unlock(&count_mtx);
    return(NULL);
}

int main(int argc, char *argv[]) {
    int i, workers, num, status = 0;
    pthread_t *threads;
    struct arguments *args;

    if(argc != 2) {
        printf("enter number of worker threads\n");
        return(0);
    }

    workers = atoi(argv[1]);
    counter_threads = workers;

    threads = (pthread_t *) calloc(sizeof(pthread_t),workers);
    args = (struct arguments *) calloc(sizeof(struct arguments),workers);
    
    for(i = 0; i < workers; i++) {
        args[i].status = AVAILABLE;
        pthread_mutex_init(&args[i].status_mutex, NULL);
        pthread_mutex_init(&args[i].tmp_mtx, NULL);
        pthread_cond_init(&args[i].x, NULL);
        pthread_create(&threads[i], NULL, prime, &args[i]);
    }

    i = workers - 1;
    while(1) {
        scanf(" %d", &num);
        if(num == 0) break;
        if(num < 0) continue;
        printf("for %d:\n", num);

        do {
            i = (i == workers - 1) ? 0 : i + 1;
            pthread_mutex_lock(&args[i].status_mutex);
            status = args[i].status;
            pthread_mutex_unlock(&args[i].status_mutex);
            printf("  status[%d](%ld) = %d\n", i, threads[i] % 1000, status);
        } while(status == NOT_AVAILABLE);

        pthread_mutex_lock(&args[i].tmp_mtx);
        pthread_mutex_lock(&args[i].status_mutex);
        args[i].num = num;
        args[i].status = NOT_AVAILABLE;
        pthread_cond_signal(&args[i].x);
        pthread_mutex_unlock(&args[i].status_mutex);
        pthread_mutex_unlock(&args[i].tmp_mtx);
    }

    pthread_mutex_lock(&count_mtx);
    for(i = 0; i < workers; i++) {
        pthread_mutex_lock(&args[i].tmp_mtx);
        args[i].num = 0;
        pthread_cond_signal(&args[i].x);
        pthread_mutex_unlock(&args[i].tmp_mtx);
    }
    pthread_cond_wait(&block_main, &count_mtx);
    pthread_mutex_unlock(&count_mtx);

    for(i = 0; i < workers; i++) {
        pthread_mutex_destroy(&args[i].status_mutex);
        pthread_mutex_destroy(&args[i].tmp_mtx);
        pthread_cond_destroy(&args[i].x);
    }

    free(threads);
    free(args);
    return(0);
}
