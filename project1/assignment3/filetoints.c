
#include <stdio.h>

/*
This program reads integers in binary form from a file and
prints them in stdout. Also, checks if the file is sorted
*/

int main() {
    int num, num_prev = 0, sorted = 1, amount = 0;
    FILE *file;

    file = fopen("input_file.txt", "rb");

    while(1) {
        fread(&num, sizeof(int), 1, file);
        if(feof(file)) break;
        if(num_prev > num) {
            sorted = 0;
        }
        printf("%d ", num);
        amount++;
        num_prev = num;
    }
    if(sorted) printf("\nALL %d NUMS SORTED!!\n", amount);
    else printf("\nNOT %d SORTED!!!\n", amount);
    return(0);
}