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
    char p0_pick, p1_pick;
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
            p1_pick = c;
        }

        if (c == '\n') {            
            /* 1 point for rock, 2 for paper, 3 for scissors */
            player_score += p1_pick - 87;  

            /* 3 points for a draw if picks are equal */
            if (p0_pick + 23 == p1_pick) {
                player_score += 3;
            }

            /* 6 points for a win if p1 pick is exactly 1 greater than or 2 smaller than opponent */
            if ((p0_pick + 23 + 1 == p1_pick) || (p0_pick + 23 - 2 == p1_pick)) {
                player_score += 6;
            }
        }
    }
    printf("Player score is %d\n", player_score);

    return 0;
}