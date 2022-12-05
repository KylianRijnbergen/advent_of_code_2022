#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stack.h"

void parse_line(char* line, int line_no, int operations[503][3]);
bool is_digit(char c);
void move(CharStack* stack[9], int operation[3]);

int main(int argc, char* argv[]) {
    /* Input of box arrangement */
    char* inp1 = "MJCBFRLH";
    char* inp2 = "ZCD";
    char* inp3 = "HJFCNGW";
    char* inp4 = "PJDMTSB";
    char* inp5 = "NCDRJ";
    char* inp6 = "WLDQPJGZ";
    char* inp7 = "PZTFRH";
    char* inp8 = "LVMG";
    char* inp9 = "CBGPFQRJ";
    char* input[9] = {inp1, inp2, inp3, inp4, inp5, inp6, inp7, inp8, inp9};


    /* Create 9 charstacks */
    CharStack* stack[9];
    for (unsigned int i = 0; i < 9; i++) {
        stack[i] = cstack();
    }


    /* Add input to charstacks */
    for (unsigned int i = 0; i < 9; i++) {
        for (unsigned int j = 0; j < strlen(input[i]); j++) {
            push(stack[i], input[i][j]);
        }
    }

    /* Input Parsing */
    int operations[503][3];
    char line[50];
    int line_no = 0;

    FILE* file;
    file = fopen("operations.txt", "r");

    while (fgets(line, 50, file) != NULL) {
        parse_line(line, line_no, operations);
        line_no++;
    }

    /* Logic */
    for (unsigned int i = 0; i < 503; i++) {
        move(stack, operations[i]);
    }
    
    /* Print answer: */
    printf("The answer is: ");
    for (int k = 0; k < 9; k++) {
        putchar(pop(stack[k]));
    }
    printf("\n");

    return 0;
}

void parse_line(char* line, int line_no, int operations[503][3]) {
    char buffer[5]; 
    char c;
    int char_pos = 0, current_start = 0, current_int = 0;

    while((c = line[char_pos]) != '\0') {
        /* As long as the character is a digit, put it in the buffer */
        if (is_digit(c)) {
            buffer[char_pos - current_start] = c;
        } else { /* If the character is not a digit, we parse the buffer. */
            buffer[char_pos - current_start] = '\0';
            if (strlen(buffer) > 0) {
                operations[line_no][current_int] = atoi(buffer);
                current_int++;
            }
            current_start = char_pos + 1;
        }
        char_pos++;
    }
}

bool is_digit(char c) {
    return c >= 48 && c <= 57;
}

void move(CharStack* stack[9], int operation[3]) {
    int number = operation[0];
    int from = operation[1] - 1;
    int to = operation[2] - 1;
    char c;

    for (int i = 0; i < number; i++) {
        c = pop(stack[from]);
        push(stack[to], c);
    }
}