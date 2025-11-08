#include <stdio.h>
#include <stdlib.h>
#include "Adat.h"

Board* head = NULL;
int main(){
    head = create_board(NULL);
    Board* board1 = add_new_board(head);
    Board* board2 = add_new_board(board1);
    Board* board3 = add_new_board(board1);
    Board* board4 = add_new_board(board2);
    Board* board5 = add_new_board(head);
    Board* board6 = add_new_board(board3);

    save_boards("avar.dat");

    felszabaditas(head);

    load_boards("avar.dat");
    save_boards("avar2.dat");
    felszabaditas(head);
}
