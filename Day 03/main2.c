#include <stdio.h>
#include <string.h>
#define MAX_LEN 60

char* find_common_char(char* elf1, char* elf2, char* elf3); 
char ALPHABET[52];

int main(int argc, char* argv[]) {
    FILE* file;
    file = fopen("input.txt", "r");
    char ELVES[3][MAX_LEN];

    /* Fill char array ALPHABET */
    for (char i = 65; i <= 90; i++) {
        ALPHABET[i-39] = i;
    }
    for (char i = 97; i <= 122; i++) {
        ALPHABET[i-97] = i;
    }
    for (int i = 0; i < 52; i++) {
        printf("ALPHABET[%d] is %c\n", i, ALPHABET[i]);
    }

    char* common_char;
    int score = 0;
    int elf_ctr = 0;

    while (fgets(ELVES[elf_ctr % 3], MAX_LEN, file) != NULL) {
        elf_ctr++;
        if ((elf_ctr % 3) == 0) {
            common_char = find_common_char(ELVES[0], ELVES[1], ELVES[2]);
            if (*common_char >= 65 && *common_char <= 90) {
                score += *common_char - 38;
            }
            if (*common_char >= 97  && *common_char <= 122) {
                score += *common_char - 96;
            }
        }
    }
    printf("Score is %d\n", score);
    return 0;
}

char* find_common_char(char* elf1, char* elf2, char* elf3) {
    for (int i = 0; i < 52; i++) {
        if ((strchr(elf1, ALPHABET[i]) != NULL 
        && strchr(elf2, ALPHABET[i]) != NULL 
        && strchr(elf3, ALPHABET[i]) != NULL)) {
            return &ALPHABET[i];
        }
    }
}