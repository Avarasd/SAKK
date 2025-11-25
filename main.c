/*
Tóth Avar Áron
DQMKMR
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "debugmalloc.h"
#include "storage.h"
#include "chess.h"
#include "display.h"

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

void booleans_init(Booleans* b){
    b->check = false;
    b->mate = false;
    b->stalemate = false;
    b->isWhiteTurn = true;
    b->white_king_moved = false;
    b->black_king_moved = false;
    b->white_rook_A_moved = false;
    b->white_rook_H_moved = false;
    b->black_rook_A_moved = false;
    b->black_rook_H_moved = false;
}

int main(){
    booleans_init(&game_booleans);
    head = create_board(NULL);
    if(head != NULL){
        memcpy(head->allas, board, sizeof(board));
    }
    Board* curr_node = head;
    while (!game_booleans.mate && !game_booleans.stalemate) {
        board_print(board);

        char move[5];
        printf("Adj meg egy lepest! \n");
        scanf("%s", move);
        if(move[0] == 'X'){
            free_all(head);
            return 0;
        }
        Input formatted_input = curr_move(move, board);
        if(is_move_pattern_valid(formatted_input, board, game_booleans.isWhiteTurn, true, game_booleans)){
            //Megnézi, hogy mozogtak-e a bástyák és királyok
            bool_checker(board, &game_booleans);

            printf("Szabalyos lepes \n");
            //Megnézi, hogy van-e gyalog az utolsó sorokban (0/7), ha van, bekér inputot, és átváltoztatja.
            pawn_promotion(formatted_input, board);

            if(curr_node != NULL){
                Board* next_node = add_new_board(curr_node);
                if(next_node != NULL){
                    memcpy(next_node->allas, board, sizeof(board));

                    curr_node = next_node;
                }
            }

            //Megváltoztatja a következő játékost
            game_booleans.isWhiteTurn = game_booleans.isWhiteTurn ? false : true;
            //Megnézi, hogy a következő játékos a lépés után sakkban van-e
            game_booleans.check = is_king_in_check(board, game_booleans.isWhiteTurn);

            if(game_booleans.check) printf("SAKK!\n");
            //Ha a játékos sehogyan sem tud szabályosat lépni, a játéknak vége

            if(!any_valid_moves(board, game_booleans.isWhiteTurn)){
                if(game_booleans.check){
                    printf("MATT! Jatek vege.\n");
                    game_booleans.mate = true;
                } else {
                    printf("PATT! Dontetlen\n");
                    game_booleans.stalemate = true;
                }

                char fajlnev[100];
                printf("Mentés fájlba... mi legyen a fájl neve? (kiterjesztés nélkül, max. 90 karakter\n)");
                scanf("%s", fajlnev);
                strcat(fajlnev, ".dat");
                save_boards(fajlnev);
                free_all(head);
            }
        }else{
            printf("Lepj ujra mert helytelen\n");
        }
    }
    return 0;   
}
