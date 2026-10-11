
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include"project1_1_pipe.h"

pipe_t *pipes[MAX_PIPES] = {NULL};

int thread_check[2] = {0};

int pipe_open(int size) {
    int i = 0;
    pipe_t *pipe_ptr = NULL;
    pipe_ptr = calloc(sizeof(pipe_t),1);

    if (pipe_ptr == NULL) {
        printf("Calloc error at %d\n",__LINE__);
        return(-1);
    }
    for (i = 0; i < MAX_PIPES; i++) {
        if (pipes[i] == NULL) {
            break;
        }
    }

    pipe_ptr->pipe_id = i;
    pipe_ptr->pipe_size = size;
    pipe_ptr->buffer = calloc(size,sizeof(char));

    if (pipe_ptr->buffer == NULL) {
        printf("Calloc error at %d\n",__LINE__);
        return(-1);
    }

    pipe_ptr->write_enable = 1;
    pipe_ptr->read = 0;
    pipe_ptr->write = 0;
    pipe_ptr->is_empty = 1;
    pipe_ptr->is_full = 0;
    pipes[i] = pipe_ptr;
    return(pipe_ptr->pipe_id);

}

void pipe_close(int p) {
    free(pipes[p]->buffer);
    free(pipes[p]);
    pipes[p] = NULL;
}



int pipe_write(int p, char c) {
    if (pipes[p] == NULL || pipes[p]->write_enable == 0) {
        return(-1);
    }

    while (pipes[p]->is_full == 1) {
        //printf("write is blocked!!\n");
    } //blocking

    pipes[p]->buffer[pipes[p]->write] = c;

    if (pipes[p]->write == pipes[p]->pipe_size - 1) {
        pipes[p]->write = 0;
    }
    else {
        pipes[p]->write = pipes[p]->write + 1;
    }

    if (pipes[p]->read == pipes[p]->write && pipes[p]->is_empty != 1) {
        pipes[p]->is_full = 1;
    }
   
    //printf("%s\n",pipes[p]->buffer);
    //printf("wrote %c in pipe %d\nwrite = %d  read = %d\n\n", c, p, pipes[p]->write, pipes[p]->read);

    pipes[p]->is_empty = 0;
    return(1);
}

int pipe_writeDone(int p) {
    if (pipes[p] == NULL || pipes[p]->write_enable == 0) {
        return(-1);
    }
    pipes[p]->write_enable = 0;
    return(1);
}

int pipe_read(int p, char *c) {
    if (pipes[p] == NULL) {
        return(-1);
    }
    if (pipes[p]->write_enable == 0 && pipes[p]->is_empty == 1) {
        pipe_close(p);
        return(0);
    }
    
    while (pipes[p]->is_empty == 1) {
        //printf("                  read is blocked!!\n");
    }
    *c = pipes[p]->buffer[pipes[p]->read];
    if (pipes[p]->read == pipes[p]->pipe_size - 1) {
        pipes[p]->read = 0;
    }
    else {
        pipes[p]->read = pipes[p]->read + 1;
    }
    
    if (pipes[p]->read == pipes[p]->write && pipes[p]->is_full != 1) {
        pipes[p]->is_empty = 1;
    }
    //printf("                  read %c from pipe %d\n                  write = %d  read = %d\n\n",
    //*c, p, pipes[p]->write, pipes[p]->read);
    pipes[p]->is_full = 0;
    return(1);
}

