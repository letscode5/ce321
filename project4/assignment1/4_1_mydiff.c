
#include <stdio.h>
#include <stdlib.h>
#include "4_1_pipe.h"

void mydiff_all_files() {
    int res1 = 0, res2 = 0, res3 = 0;
    FILE *file1, *file2, *file3;

    file1 = fopen(OUR_FILE,"rb");
    if (file1 == NULL) {
        printf("Error reading file 1\n");
    }
    file2 = fopen("file2.txt","rb");
    if (file2 == NULL) {
        printf("Error reading file 2\n");
    }
    file3 = fopen("file3.txt","rb");
    if (file3 == NULL) {
        printf("Error reading file 3\n");
    }

    res1 = compare_files(file1, file2);
    rewind(file1); rewind(file2);
    res2 = compare_files(file2, file3);
    rewind(file3);
    res3 = compare_files(file1, file3);

    if(res1 && res2 && res3) {
        printf("All files are the same\n");
    }
    else {
        printf("Files not the same\n");
    }
    fclose(file1);
    fclose(file2);
    fclose(file3);
}

/*  returns 0 if not the same,
    returns 1 if the same */
int compare_files(FILE * fPtr1, FILE * fPtr2) {
    char ch1, ch2;

    do {
        ch1 = fgetc(fPtr1);
        ch2 = fgetc(fPtr2);
        if (ch1 != ch2) {
            return 0;
        }
    } while (ch1 != EOF && ch2 != EOF);

    if (ch1 == EOF && ch2 == EOF) {
        return 1;
    }
    else {
        return 0;
    }
}