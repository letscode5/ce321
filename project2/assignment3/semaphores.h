#ifndef __2_1_H_
#define __2_1_H_

struct mysem_t_ {
    int semid;
    int init;
    int up_called;
    int down_called;
    struct sembuf op;
};

typedef struct mysem_t_ mysem_t;

extern int mysem_init(mysem_t *s, int n);

extern int mysem_down(mysem_t *s);

extern int mysem_up(mysem_t *s);

extern int mysem_destroy(mysem_t *s);

extern void mysem_calloc(mysem_t **s);


#endif
