#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "sort.h"

#define MAX_LEN 20

int main(int argc, char* argv[]) {
    FILE* file; 
    file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    long long current_elf_cals = 0;
    long long current_item_cals;
    char buff[MAX_LEN]; 
    int newlines = 0;
    const int NR_ELVES = 500;
    int current_elf = 0;
    long long elves[NR_ELVES];
    for (int i = 0; i < NR_ELVES; i++) {
        elves[i] = 0;
    }

    char c; 
    int char_pos = 0;

    while((c = fgetc(file)) != EOF) {
        if(c == '\n') {
            buff[char_pos] = '\0';
            current_item_cals = atoll(buff);
            current_elf_cals += current_item_cals;
            newlines++;
            char_pos = 0;
        } else {
            buff[char_pos] = c;
            newlines = 0;
            char_pos++;
        }
        /* If we have more than one new line, we have a new elf. */
        if (newlines > 1) { 
            elves[current_elf] = current_elf_cals;
            current_elf_cals = 0;
            current_elf++;
        }
    }
    quicksort_ll(elves, NR_ELVES);
    printf("The top three elves are carrying %lld, %lld, and %lld. This is %lld calories in total. \n", elves[497], elves[498], elves[499], elves[497] + elves[498] + elves[499]);
    return 0;
}