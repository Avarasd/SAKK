/*
Tóth Avar Áron
DQMKMR
*/

typedef struct board {
    char allas[8][8];
    struct board* prev;
    int numNext;
    struct board** next;
    int id;
    int previd;

} Board;

extern Board* head;

Board* add_new_board(Board *curr);

Board* create_board(Board* previous);

Board* add_new_board(Board *curr);

void free_all(Board* curr);

void save_boards(char* filename);

void load_boards(char* filename);

Board* find_last_board(Board* curr);

Board* search_board_linear(Board* head, int move_count);