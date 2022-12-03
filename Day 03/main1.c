#include <stdio.h>
#include <string.h>
#define MAX_LEN 60

void split_string_in_half(char* src, char* dest1, char* dest2);

int main(int argc, char* argv[]) {
    FILE* file;
    file = fopen("input.txt", "r");
    char elf[MAX_LEN];
    char p0[MAX_LEN/2]; 
    char p1[MAX_LEN/2]; 

    char* common_char;
    int score = 0;

    while (fgets(elf, MAX_LEN, file) != NULL) {
        split_string_in_half(elf, p0, p1);
        common_char = strpbrk(p0, p1);
        if (*common_char >= 65 && *common_char <= 90) {
            score += *common_char - 38;
        }
        if (*common_char >= 97  && *common_char <= 122) {
            score += *common_char - 96;
        }
    }
    printf("Score is %d\n", score);
    return 0;
}

void split_string_in_half(char* src, char* dest1, char* dest2) {
    int str_len = strlen(src);
    strncpy(dest1, src, str_len/2);
    dest1[str_len/2] = '\0';
    strncpy(dest2, &src[str_len/2], str_len/2);
    dest2[str_len/2] = '\0';
}