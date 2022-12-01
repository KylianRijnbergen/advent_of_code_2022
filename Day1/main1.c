#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LEN 20

int main(int argc, char* argv[]) {
    FILE* file; 
    file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    long long current_elf_cals = 0;
    long long most_cals = 0;
    long long current_item_cals;
    char buff[MAX_LEN]; 
    int newlines = 0;

    char c; 
    int char_pos = 0;

    while((c = fgetc(file)) != EOF) {
        if(c == '\n') {
            buff[char_pos] = '\0';
            current_item_cals = atoll(buff);

            current_elf_cals += current_item_cals;
            if (current_elf_cals > most_cals) {
                most_cals = current_elf_cals;
            }
            newlines++;
            char_pos = 0;
        } else {
            buff[char_pos] = c;
            newlines = 0;
            char_pos++;
        }
        if (newlines > 1) {
            current_elf_cals = 0;
        }
    }
    printf("Most calories carried by an elf is %lld.\n", most_cals);
    return 0;
}