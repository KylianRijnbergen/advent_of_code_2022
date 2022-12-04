#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

void parse_line(char* line, int* ranges);
bool has_overlap(int* ranges);
bool is_digit(char c);

int main(int argc, char* argv[]) {
    int ranges[4];
    char line[15];
    int score = 0;

    FILE* file;
    file = fopen("input.txt", "r");
    while (fgets(line, 15, file) != NULL) {
        parse_line(line, ranges);
        if (has_overlap(ranges)) {
            score++;
        }
    }
    printf("Score is %d\n", score);
    return 0;
}

void parse_line(char* line, int* ranges) {
    char buffer[5]; 
    char c;
    int char_pos = 0, current_start = 0, current_int = 0;

    while((c = line[char_pos]) != '\0') {
        /* As long as the character is a digit, put it in the buffer */
        if (is_digit(c)) {
            buffer[char_pos - current_start] = c;
        } else { /* If the character is not a digit, we parse the buffer. */
            buffer[char_pos - current_start] = '\0';
            ranges[current_int] = atoi(buffer);
            current_int++;
            current_start = char_pos + 1;
        }
        char_pos++;
    }
}

bool has_overlap(int* ranges) {
    bool overlap;
    /* If the range of the first elf is strictly smaller than the second elf, we have nu overlap. */
    if (ranges[0] < ranges[2] && ranges[1] < ranges[2]) {
        return false;
    }
    /* Similarly, if the range of the secodn elf is strictly smaller than the first elf, we have no overlap. */
    if (ranges[2] < ranges[0] && ranges[3] < ranges[0]) {
        return false;
    }
    /* Otherwise, we have overlap. */
    return true;
}

bool is_digit(char c) {
    return c >= 48 && c <= 57;
}