
#ifndef __HW1_PIPE_H_
#define __HW1_PIPE_H_

#define MAX_PIPES 5
#define PIPE_SIZE 64
#define OUR_FILE "bigfile.txt"

extern int thread_check[2];

struct pipe_t_ {
    int pipe_id;
    int pipe_size;
    char *buffer;
    int write_enable;
    int write;
    int read;
    int is_empty;
    int is_full;
};


typedef struct pipe_t_ pipe_t;


struct arguments {
    int pipe_id1;
    int pipe_id2;
};

typedef struct arguments arguments_t;

extern int pipe_open(int size);

extern void pipe_close(int p);

extern int pipe_write(int p, char c);

extern int pipe_writeDone(int p);

extern int pipe_read(int p, char *c);

extern void *thread_1(void *arg);

extern void *thread_2(void *arg);

#endif



