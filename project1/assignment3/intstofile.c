
#include<stdio.h>

#define NUM_INTEGERS 20

/*
This program takes an input of integers and writes them into a file
as integers in binary form
*/

int main(int argc,char *argv[]) {
    int num,res;
    FILE *file;
    file = fopen("input_file.txt","wb+");
    if (file == NULL) {
        printf("fopen failed at %d",__LINE__ - 1);
    }
    while(1) {
        scanf("%d",&num);
        if (num == -1) {
            break;
        }
        res = fwrite(&num,sizeof(int),1,file);
        if (res != 1) {
            printf("Fwrite failed at %d",__LINE__ - 1);
        }
    }
    fclose(file);
    return(0);

}
