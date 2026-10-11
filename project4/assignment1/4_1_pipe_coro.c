
#include <stdio.h>
#include <stdlib.h>
#include "4_1_pipe.h"
#include "4_1_coro.h"

void coroutine1(void *arg) {
    char c;
    int pipe_id1,pipe_id2,res,i;
    long file_start, file_end, file_size;
    FILE *file1,*file3;
    arguments_t *arg_ptr = (arguments_t*) arg;
    pipe_id1 = arg_ptr->pipe_id1;
    pipe_id2 = arg_ptr->pipe_id2;

    file1 = fopen(OUR_FILE,"rb");
    if (file1 == NULL) {
        printf("Error reading file 1\n");
    }
    res = fseek(file1,0,SEEK_END);
    if (res != 0) {
        printf("Problem with fseek at %d\n",__LINE__);
    }
    file_end = ftell(file1);
    if (file_end == -1) {
        printf("Problem with ftell at %d\n",__LINE__);
    }
    rewind(file1);
    file_start = ftell(file1);
    file_size = file_end - file_start;

    for (i = 0; i < file_size; i++) {
        res = fread(&c,1,1,file1);
        if (res != 1) {
            printf("Problem with fread at %d\n",__LINE__);
        }
        pipe_write(pipe_id1,c);
    }
    pipe_writeDone(pipe_id1);


    file3 = fopen("file3.txt","wb");
    while (1) {
        if (pipe_read(pipe_id2,&c) == 0) {
            break;
        }
        res = fwrite(&c,1,1,file3);
        if (res != 1) {
            printf("Problem with fwrite at %d\n",__LINE__);
        }
    }
    fclose(file1);
    fclose(file3);
    mycoroutines_switchto(&co2);
}

void coroutine2(void *arg) {
    char c;
    int pipe_id1,pipe_id2,res,i;
    long file_size;
    FILE *file2;

    arguments_t *arg_ptr = (arguments_t*) arg;
    pipe_id1 = arg_ptr->pipe_id1;
    pipe_id2 = arg_ptr->pipe_id2;

    file2 = fopen("file2.txt","wb+");

    while (1) {
        if (pipe_read(pipe_id1,&c) == 0) {
            break;
        }
        res = fwrite(&c,1,1,file2);
        if (res != 1) {
            printf("Problem with fwrite at %d\n",__LINE__);
        }
    }
    file_size = ftell(file2);

    rewind(file2);

    for (i = 0; i < file_size; i++) {
        res = fread(&c,1,1,file2);
        if (res != 1) {
            printf("Problem with fread at %d\n",__LINE__);
        }
        pipe_write(pipe_id2,c);
    }
    pipe_writeDone(pipe_id2);
    fclose(file2);
    mycoroutines_switchto(&main_co);
}