
/*
Maris Athanasios        03045
Sialtsis Alexandros     03083

This program takes a file as an argument and because the
file contents do not fit in the memory and uses external merge sort
using threads to sort it.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_FILE_NAME 30
#define MAX_CHUNK 64

//status = 0  ----> thread is finished
struct arguments {
    FILE *file;
    int start;
    int end;
    int *status;
};

void merge_sort(int a[],int left,int right,int b[]) {
    int l,r,middle = (right + left) / 2;

    if (left < middle) {
        merge_sort(a,left,middle,b);
    }
    if (middle+1 < right) {
        merge_sort(a,middle+1,right,b);
    }
    for (l = left; l < middle + 1; l++) {
        b[l] = a[l];
    }
    l = left;
    for (r = middle; r < right; r++) {
        b[right + middle - r] = a[r + 1];
    }
    for (int k = left; k <= right; k++) {
        if (b[r] < b[l]) {
            a[k] = b[r--];
        }
        else {
            a[k] = b[l++];
        }
    }
}

void *external_merge_sort(void *arg) {
    struct arguments *args = arg, new_args1, new_args2;
    pthread_t thread1, thread2;
    FILE *file = args->file, *new_file1 = NULL, *new_file2 = NULL;
    int start = args->start, end = args->end, *status = args->status; 
    int *buffer, *buffer2, size = end - start + sizeof(int);
    int status1 = 1, status2 = 1, i, integer1, integer2;
    char file1[MAX_FILE_NAME] = {'\0'}, file2[MAX_FILE_NAME] ={'\0'};

    if(size <= MAX_CHUNK) {
        buffer = (int *) calloc(1, size);
        buffer2 = (int *) calloc(1, size);
        if(buffer == NULL) printf("malloc failed at %d\n", __LINE__ - 2);
        if(buffer2 == NULL) printf("calloc failed at %d\n", __LINE__ - 2);
        rewind(file);
        if(fread(buffer, size, 1, file) != 1) printf("fread failed at %d\n", __LINE__);
        merge_sort(buffer, 0, size / sizeof(int) - 1, buffer2);
        rewind(file);
        if(fwrite(buffer, size, 1, file) != 1) printf("fwrite failed at %d\n", __LINE__);
        status1 = status2 = 0;
        free(buffer2);
    }
    else {
        new_args1.start = 0;
        new_args1.end = (size % 8 == 0) ? size / 2 - sizeof(int) : size / 2 - ( (3 * sizeof(int) ) / 2);
        new_args2.start = 0;
        new_args2.end = (size % 8 == 0) ? size / 2 - sizeof(int) : size / 2 - (sizeof(int) / 2);

        new_args1.status = &status1;
        new_args2.status = &status2;

        if(sprintf(file1, "merge%lu_1", pthread_self()) < 0) printf("sprintf failed at %d\n", __LINE__);
        if(sprintf(file2, "merge%lu_2", pthread_self()) < 0) printf("sprintf failed at %d\n", __LINE__);
        new_file1 = fopen(file1, "wb+");
        if(new_file1 == NULL) printf("fopen failed at %d\n", __LINE__ - 1);
        new_file2 = fopen(file2, "wb+");
        if(new_file2 == NULL) printf("fopen failed at %d\n", __LINE__ - 1);

        rewind(file);
        rewind(new_file1);
        rewind(new_file2);
        buffer = (int *) malloc(MAX_CHUNK);
        for(i = 0; i <= new_args1.end / sizeof(int); i++) {
            if(fread(buffer, sizeof(int), 1, file) != 1) printf("fread failed at %d\n", __LINE__);
            if(fwrite(buffer, sizeof(int), 1, new_file1) != 1) printf("fwrite failed at %d\n", __LINE__); 
        }
        for(i = 0; i <= new_args2.end / sizeof(int); i++) {
            if(fread(buffer, sizeof(int), 1, file) != 1) printf("fread failed at %d\n", __LINE__);
            if(fwrite(buffer, sizeof(int), 1, new_file2) != 1) printf("fwrite failed at %d\n", __LINE__); 
        }

        new_args1.file = new_file1;
        new_args2.file = new_file2;

        pthread_create(&thread1, NULL, external_merge_sort, &new_args1);
        pthread_create(&thread2, NULL, external_merge_sort, &new_args2);

        while(status1 == 1 || status2 == 1) {/*NOP*/}
    

        rewind(file);
        rewind(new_file1);
        rewind(new_file2);
        
        if(fread(&integer1, sizeof(int), 1, new_file1) != 1) printf("fread failed at %d\n", __LINE__);
        if(fread(&integer2, sizeof(int), 1, new_file2) != 1) printf("fread failed at %d\n", __LINE__);
        while(1) {
            if(integer1 < integer2) {
                if(fwrite(&integer1, sizeof(int), 1, file) != 1) printf("fwrite failed at %d\n", __LINE__);
                if(fread(&integer1, sizeof(int), 1, new_file1) != 1 && !feof(new_file1)) printf("fread failed at %d\n", __LINE__);
                if(feof(new_file1)) {
                    if(fwrite(&integer2, sizeof(int), 1, file) != 1) printf("fwrite failed at %d\n", __LINE__);
                    while(1) {
                        if(fread(&integer2, sizeof(int), 1, new_file2) != 1 && !feof(new_file2)) printf("fread failed at %d\n", __LINE__);
                        if(feof(new_file2)) break;
                        if(fwrite(&integer2, sizeof(int), 1, file) != 1) printf("fwrite failed at %d\n", __LINE__);
                    }
                    break;
                }
            }
            else {
                if(fwrite(&integer2, sizeof(int), 1, file) != 1) printf("fwrite failed at %d\n", __LINE__);
                if(fread(&integer2, sizeof(int), 1, new_file2) != 1 && !feof(new_file2)) printf("fread failed at %d\n", __LINE__);
                if(feof(new_file2)) {
                    if(fwrite(&integer1, sizeof(int), 1, file) != 1) printf("fwrite failed at %d\n", __LINE__);
                    while(1) {
                        if(fread(&integer1, sizeof(int), 1, new_file1) != 1 && !feof(new_file1)) printf("fread failed at %d\n", __LINE__);
                        if(feof(new_file1)) break;
                        if(fwrite(&integer1, sizeof(int), 1, file) != 1) printf("fwrite failed at %d\n", __LINE__);
                    }
                    break;
                }
            }
        }

        fclose(new_file1); fclose(new_file2);
        remove(file1); remove(file2);
    }
    
    free(buffer);
    *status = 0;
    return(NULL);
}

int main(int argc, char *argv[]) {
    FILE *file;
    char filename[MAX_FILE_NAME] = {'\0'};
    pthread_t thread;
    int file_size;
    struct arguments args;
    int status = 1;

    if(argc != 2) {
        printf("give a file to sort!!\n");
        return(0);
    }

    strcpy(filename, argv[1]);
    file = fopen(filename, "rb+");
    if(file == NULL) printf("fopen failed at %d\n", __LINE__ - 1);

    if(fseek(file, 0L, SEEK_END) == -1) printf("fseek failed at %d\n", __LINE__);
    file_size = ftell(file);
    if(file_size == -1) printf("ftell failed at %d\n", __LINE__ - 1);
    rewind(file);

    args.start = 0;
    args.end = file_size - sizeof(int);
    args.status = &status;
    args.file = file;

    pthread_create(&thread, NULL, external_merge_sort, &args);
    
    while(1) {
        if(*args.status == 0) {
            fclose(file);
            return(0);
        }
    }
}