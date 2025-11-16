#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "storage.h"
#include "chess.h"

Board* head = NULL;
Booleans game_booleans;
char board[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
};

int main(){
    game_booleans.check = false;
    game_booleans.mate = false;
    game_booleans.stalemate = false;

    while (!game_booleans.mate) {
        char move[5];
        for (int sor = 7; sor >= 0; sor--) {
            for (int elem = 0; elem < 8; elem++) {
                printf("%c ", board[sor][elem]);
            }
            printf("%d ",sor + 1);
            printf(" \n");
        }

        printf("A B C D E F G H \n\n");
        printf("Adj meg egy lepest! \n");
        scanf("%s", move);
        if(move[0] == 'X') return 0;
        Input formatted_input = curr_move(move, board);

        if(is_move_pattern_valid(formatted_input, board)){
            printf("Szabalyos lepes \n");
        }else{
            printf("Lepj ujra mert helytelen\n");
        }
    }

    return 0;   
}
