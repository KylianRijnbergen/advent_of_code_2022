#include <stdio.h>
#include <stdlib.h> 

int main(int argc, char* argv[]) {
    FILE* file; 
    file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    int player_turn = 0;
    int player_score = 0;
    char p0_pick, p1_pick, p1_obj;
    char c;

    while((c = getc(file)) != EOF) {
        /* Swap player turns if space  */
        if (c == ' ') {
            if (player_turn == 0) { 
                player_turn = 1;
            } else {
                player_turn = 0;
            }
        }

        if (c == 'A' || c == 'B' || c == 'C') {
            p0_pick = c;
        } 

        if (c == 'X' || c == 'Y' || c == 'Z' ) {
            p1_obj = c - 88;
        }

        if (c == '\n') {            
            /* 0 points for loss, 3 for draw, 6 for win */  
            player_score += 3 * p1_obj;
            if (p1_obj == 0) {
                if (p0_pick == 'A') {
                    player_score += 3;
                }
                if (p0_pick == 'B') {
                    player_score += 1; 
                }
                if (p0_pick == 'C') {
                    player_score += 2;
                }
            }

            if (p1_obj == 1) {
                player_score += p0_pick - 64;
            }

            if (p1_obj == 2) {
                if (p0_pick == 'A') {
                    player_score += 2;
                }
                if (p0_pick == 'B') {
                    player_score += 3;
                }
                if (p0_pick == 'C') {
                    player_score += 1;
                }
            }
        }
    }
    printf("Player score is %d\n", player_score);
    return 0;
}