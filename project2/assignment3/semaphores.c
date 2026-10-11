
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

int proj_id = 1;

void *thread1(void *arg) {
    int i;
    mysem_t *s1 = (mysem_t *) arg;

    for(i = 0; i < 3; i++) {
        mysem_down(s1);
        printf("\n1:\n");
        printf("i'm supposed to print\n");
        printf("this\n");
        printf("...\n");
        sleep(5);
        printf("and this\n");
        printf("without getting interrupted\n\n");
        mysem_up(s1);
    }
    return(NULL);
}

void *thread2(void *arg) {
    int i;
    mysem_t *s1 = (mysem_t *) arg;

    for(i = 0; i < 3; i++) {
        mysem_down(s1);
        printf("\n2:\n");
        printf("i'm supposed to print\n");
        printf("this\n");
        printf("...\n");
        sleep(5);
        printf("and this\n");
        printf("without getting interrupted\n\n");
        mysem_up(s1);
    }
    return(NULL);
}

void *thread3(void *arg) {
    int i;
    mysem_t *s1 = (mysem_t *) arg;

    for(i = 0; i < 3; i++) {
        mysem_down(s1);
        printf("\n3:\n");
        printf("i'm supposed to print\n");
        printf("this\n");
        printf("...\n");
        sleep(5);
        printf("and this\n");
        printf("without getting interrupted\n\n");
        mysem_up(s1);
    }
    return(NULL);
}

int mysem_init(mysem_t *s, int n) {
    key_t key;

    if (n < 0 || n > 1) {
        return(0);
    }
    if(s->init != 0) {
        return(-1);
    }
    key = ftok("dummy.txt",proj_id);
    if (key == -1) {
        printf("Error at ftok line %d\n",__LINE__ - 2);
    }
    proj_id++;
    s->semid = semget(key, 1, 0666 | IPC_CREAT);
    if (s->semid == -1) {
        printf("Error at semget line %d\n",__LINE__ - 2);
    }
    if (semctl(s->semid,0,SETVAL,n) == -1) {
        printf("Error at semctl line %d\n",__LINE__ - 2);
        printf("Errno is %d\n",errno);
    }
    s->init = 1;
    //printf("Semid = %d\n",s->semid);
    return(1);
}

int mysem_down(mysem_t *s) {
    if(s->init == 0) {
        printf("down: not init\n");
        return(-1);
    }
    //printf("dowm:    ");
    s->op.sem_num = 0;
    s->op.sem_op = -1;
    s->op.sem_flg = 0;
    //printf("old sem = %d", semctl(s->semid, 0, GETVAL));
    semop(s->semid, &s->op, 1);
    //printf("    new sem = %d\n", semctl(s->semid, 0, GETVAL));
    return(1);
}

int mysem_up(mysem_t *s) {
    if(s->init == 0) {
        printf("down: not init\n");
        return(-1);
    }
    //printf("up:      ");
    s->op.sem_num = 0;
    s->op.sem_op = 1;
    s->op.sem_flg = 0;
    //printf("old sem = %d", semctl(s->semid, 0, GETVAL));
    if(semctl(s->semid, 0, GETVAL) == 0) {
        semop(s->semid, &s->op, 1);
    }
    else {
        //printf("    new sem = %d\n", semctl(s->semid, 0, GETVAL));
        return(0);
    }
    //printf("    new sem = %d\n", semctl(s->semid, 0, GETVAL));
    return(1);
}

int mysem_destroy(mysem_t *s) {
    if(s->init == 0) {
        return(-1);
    }
    if(semctl(s->semid, 0, IPC_RMID) == -1) {
        printf("Error at semctl at %d\n", __LINE__ - 1);
    }
    //printf("DESTRUCTIONNN!!!\n");
    return(1);
}

void mysem_calloc(mysem_t **s) {
    *s = calloc(1, sizeof(mysem_t));
    if(*s == NULL) {
        printf("Error at calloc at %d\n", __LINE__ - 2);
    }
}

/*int main(int argc,char *argv[]) {
    mysem_t *s1;
    pthread_t t1, t2, t3;

    mysem_calloc(&s1);

    mysem_init(s1, 1);

    mysem_down(s1);
    mysem_down(s1);
    mysem_up(s1);
    mysem_up(s1);
    mysem_down(s1);
    mysem_up(s1);
    mysem_down(s1);
    mysem_down(s1);

    pthread_create(&t1, NULL, thread1, (void *) s1);
    pthread_create(&t2, NULL, thread2, (void *) s1);
    pthread_create(&t3, NULL, thread3, (void *) s1);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    mysem_destroy(s1);
    free(s1);
    
    return(0);
}*/












