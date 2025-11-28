/*
Tóth Avar Áron
DQMKMR
*/

typedef struct board {
    char board[8][8];
    struct board* prev;
    int numNext;
    struct board** next;
    int id;
    int previd;
    int selectedBranch;
    int en_passant_col;
} Board;


Board* add_new_board(Board *curr);

Board* create_board(Board* previous);

void free_all(Board* curr);

void save_boards(char* filename, Board* head);

void load_boards(char* filename, Board** head);

Board* find_last_board(Board* curr);

Board* search_board_linear(Board* head, int move_count);

int find_last_board_move_count(Board* head);