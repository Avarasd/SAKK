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
    b->playing = true;
}

void all_alternative_moves(Board* curr){
    char (*allMoves)[5] = malloc(curr->numNext * sizeof(char[5]));
    int board_index = 0;
    for(board_index = 0; board_index < curr->numNext; board_index++){
        reconstruct_move(curr->next[board_index]->board, curr->board, allMoves[board_index]);
    }

    display_all_alternative_moves(allMoves, curr->numNext);

    free(allMoves);
}

void bool_checker_reverse(Board* board, Booleans* b){
    Board* curr_board = board;
    while(curr_board->prev != NULL){
        if(curr_board->board[0][4] != 'k') b->white_king_moved = true;
        if(curr_board->board[7][4] != 'K') b->black_king_moved = true;

        if(curr_board->board[0][0] != 'r') b->white_rook_A_moved = true;
        if(curr_board->board[7][0] != 'R') b->black_rook_A_moved = true;

        if(curr_board->board[0][7] != 'r') b->white_rook_H_moved = true;
        if(curr_board->board[7][7] != 'R') b->black_rook_H_moved = true;
        curr_board = curr_board->prev;
    }
}

void load_all_moves(Board* head){
    Board* curr = head;
    int move_count = 1;
    while(curr->numNext > 0){
        char move[5];
        reconstruct_move(curr->next[0]->board, curr->board, move);
        Input formatted_input = curr_move(move, curr->board);
        char formatted_move[8];
        format_chess_notation(formatted_input, formatted_move);
        update_moves(formatted_move, move_count);
        curr = curr->next[0];
        move_count++;
    }
}

State game_mode_display(int minutes){
    if(head != NULL) free_all(head);
    head = create_board(NULL);
    if(head != NULL) memcpy(head->board, board, sizeof(board));
    Board* curr_node = head;
    booleans_init(&game_booleans);
    display_clear();
    int move_count = 1;
    while (!game_booleans.mate && !game_booleans.stalemate && game_booleans.playing) {

        display_board(board, 32, 5);
        display_info(game_booleans.isWhiteTurn, move_count);

        char move[5];
        display_get_input(move);
        Input formatted_input = curr_move(move, board);
        if(is_move_pattern_valid(formatted_input,board, game_booleans.isWhiteTurn, true, game_booleans)){
            bool_checker(board, &game_booleans);
            char formatted_move[8];
            format_chess_notation(formatted_input, formatted_move);
            if(pawn_promotion(formatted_input, board)){
                char choice = display_ask_promotion();
                do_promotion(formatted_input, board, choice);
            };
            update_moves(formatted_move, move_count);

            if(curr_node != NULL){
                Board* next_node = add_new_board(curr_node);
                if(next_node != NULL){
                    memcpy(next_node->board, board, sizeof(board));

                    curr_node = next_node;
                }
            }
            //Megváltoztatja a következő játékost
            game_booleans.isWhiteTurn = game_booleans.isWhiteTurn ? false : true;
            move_count += 1;
            //Megnézi, hogy a következő játékos a lépés után sakkban van-e
            game_booleans.check = is_king_in_check(board, game_booleans.isWhiteTurn);

            if(!any_valid_moves(board, game_booleans.isWhiteTurn)){
                    if(game_booleans.check){
                        game_booleans.mate = true;
                    } else {
                        game_booleans.stalemate = true;
                    }
                    display_game_state(true, game_booleans.check, game_booleans.mate, game_booleans.stalemate);
                    display_board(board, 32, 5);
                    display_sleep(5);
            }else display_game_state(true, game_booleans.check, game_booleans.mate, game_booleans.stalemate);
        } else {
            display_game_state(false, game_booleans.check, game_booleans.mate, game_booleans.stalemate);
        }
    }
    return STATE_GAME_END;
}

State anal_mode_display(void){
    if(head == NULL){
        return STATE_ANALYSIS_MENU;
    }
    Board* curr_board = head;
    display_clear();
    int move_count = 0;
    Booleans anal_booleans;
    booleans_init(&anal_booleans);
    load_all_moves(head);
    while(true){
        display_board(curr_board->board, 32, 5);
        all_alternative_moves(curr_board);
        int input = display_anal_info();
        switch(input){
            case 'l': {
                if(curr_board != head){ 
                    curr_board = curr_board->prev; 
                    move_count--; 
                }
                break;
            }
            case 'r': {
                if(curr_board->numNext != 0){
                    if(curr_board->selectedBranch >= curr_board->numNext) curr_board->selectedBranch = 0;
                    curr_board = curr_board->next[curr_board->selectedBranch];
                    move_count++; 
                }
                break;
            }
            case '1': {
                curr_board = head; 
                move_count = 0;
                break;
            }
            case '2': {
                curr_board = find_last_board(curr_board);
                move_count = find_last_board_move_count(head);
                break;
            }
            case '3': {
                move_count = display_get_char();
                if(move_count < 0) move_count = 1; //TODO WARNING ÜZENET
                if(move_count > find_last_board_move_count(head)) move_count = find_last_board_move_count(head);
                Board* result = search_board_linear(head, move_count);
                if(result != NULL) curr_board = result;
                break;
            }
            case '4':{
                char move[5];
                display_get_input(move);
                Input formatted_input = curr_move(move, curr_board->board);
                bool_checker_reverse(curr_board, &anal_booleans);
                if(is_move_pattern_valid(formatted_input,curr_board->board, !(move_count%2), false, anal_booleans)){
                    Board* new_board = add_new_board(curr_board);
                    if(new_board != NULL){
                        memcpy(new_board->board, curr_board->board, sizeof(curr_board->board));
                        is_move_pattern_valid(formatted_input, new_board->board, !(move_count%2), true, anal_booleans);
                        if(pawn_promotion(formatted_input, new_board->board)){
                            char choice = display_ask_promotion();
                            do_promotion(formatted_input, new_board->board, choice);
                        }
                        anal_booleans.check = is_king_in_check(new_board->board, !(move_count%2));
                        curr_board = new_board;
                        move_count += 1;
                    } else{
                        //TODO HIBAÜZENET: SIKERTELEN BOARD LÉTREHOZÁS
                    }
                } else {
                    //TODO DISPLAY SZABÁLYTALAN LÉPÉS
                }
            } break;
            case '5': {
                if(curr_board->numNext > 0){
                    int choice = display_get_branch();
                    if(choice > 0 && choice <= curr_board->numNext){
                        curr_board->selectedBranch = choice - 1;
                        curr_board = curr_board->next[choice - 1];
                        move_count++;
                    } else {
                        //TODO HIBAÜZENET
                    }
                }
                break;
            }
            case 'x': return STATE_ANALYSIS_END; break;
        }
    }
}

int main(){
    display_init();
    State currentState = STATE_MAIN_MENU;
    int minutes = 0;
    char fileName[200];
    bool exited = false;
    while(!exited){
        switch(currentState){
            case STATE_MAIN_MENU:
                currentState = display_menu();
                break;
            case STATE_GAME_MENU:
                currentState = game_mode_menu();
                break;
            case STATE_GAME_SETUP:
                minutes = game_mode_time_set();
                currentState = STATE_GAME_RUNNING;
                break;
            case STATE_GAME_RUNNING:
                currentState = game_mode_display(minutes);
                break;
            case STATE_ANALYSIS_MENU:
                currentState = anal_mode_menu();
                break;
            case STATE_ANALYSIS_SETUP:
                if(anal_mode_file_set(fileName)){
                    currentState = STATE_ANALYSIS_RUNNING;
                    load_boards(fileName);
                }
                else currentState = STATE_ANALYSIS_MENU;
                break;
            case STATE_ANALYSIS_RUNNING:
                currentState = anal_mode_display();
                break;
            case STATE_GAME_END:
                currentState = game_mode_end(fileName);
                save_boards(fileName);
                free_all(head);
                head = NULL;
                break;
            case STATE_ANALYSIS_END:
                currentState = STATE_MAIN_MENU;
                save_boards(fileName);
                free_all(head);
                head = NULL;
                break;
            case STATE_EXIT:
                exited = true;
                break;
        }
    }
    return 0;   
}
