#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Tree {
    int height;
    bool visible;
} Tree;

typedef struct Forest {
    int visible_trees;
    Tree** trees;
} Forest;

void load_forest(Forest* forest, int rows, int cols);
void scan(Forest* forest, int rows, int cols);
void look_n(Forest* forest, int rows, int cols);
void look_e(Forest* forest, int rows, int cols);
void look_s(Forest* forest, int rows, int cols);
void look_w(Forest* forest, int rows, int cols);
int count_visible(Forest* forest, int rows, int cols);


int main(int argc, char* argv[]) {
    int row_size = 99;
    int column_size = 99;

    Forest* forest;
    forest = malloc(sizeof(Forest));
    forest->visible_trees = 0; 
    forest->trees = malloc(sizeof(Tree*) * column_size);

 
    for (unsigned int i = 0; i < row_size; i++) {
        for (unsigned int j = 0; j < column_size; j++) {
            forest->trees[i] = malloc(sizeof(Tree) * row_size);
        }
    }
    load_forest(forest, row_size, column_size);
    scan(forest, row_size, column_size);
    printf("There are %d visible trees\n", count_visible(forest, row_size, column_size));
    return 0;
}

void load_forest(Forest* forest, int rows, int cols) {
    FILE* file;
    file = fopen("input.txt", "r");
    int counter = 0;
    int row, column;
    char c; 
    while((c = fgetc(file)) != EOF) {
        if (c != '\n') {
            row = counter / cols;
            column = counter % rows;
            forest->trees[row][column].height = c - 48;
            forest->trees[row][column].visible = false;
            counter++;
        }
    }
    fclose(file);
}

void scan(Forest* forest, int rows, int cols) {
    look_n(forest, rows, cols);
    look_e(forest, rows, cols);
    look_s(forest, rows, cols);
    look_w(forest, rows, cols);
}
void look_n(Forest* forest, int rows, int cols) {
    for (int c = 0; c < cols; c++) {
        int highest = -1;
        for (int r = 0; r < rows; r++) {
            if (forest->trees[r][c].height > highest) {
                forest->trees[r][c].visible = true;
                highest = forest->trees[r][c].height;
            }
        }
    }
}

void look_e(Forest* forest, int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        int highest = -1;
        for (int c = 0; c < cols; c++) {
            if (forest->trees[r][cols-c-1].height > highest) {
                forest->trees[r][cols-c-1].visible = true;
                highest = forest->trees[r][cols-c-1].height;
            }
        }
    }
}

void look_s(Forest* forest, int rows, int cols) {
    for (int c = 0; c < cols; c++) {
        int highest = -1;
        for (int r = 0; r < rows; r++) {
            if (forest->trees[rows-r-1][c].height > highest) {
                forest->trees[rows-r-1][c].visible = true;
                highest = forest->trees[rows-r-1][c].height;
            }
        }
    }
}

void look_w(Forest* forest, int rows, int cols) {
    for (int r = 0; r < rows; r++) {
        int highest = -1;
        for (int c = 0; c < cols; c++) {
            if (forest->trees[rows-r-1][c].height > highest) {
                forest->trees[rows-r-1][c].visible = true;
                highest = forest->trees[rows-r-1][c].height;
            }
        }
    }
}

int count_visible(Forest* forest, int rows, int cols) {
    int visible = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (forest->trees[i][j].visible) {
                visible++;
            }
        }
    }
    return visible;
}