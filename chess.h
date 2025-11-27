/*
Tóth Avar Áron
DQMKMR
*/

extern char board[8][8];
#define SMALL_CAP_DISTANCE 32

typedef struct input
{
    int orig_row;
    int orig_column;
    int target_row;
    int target_column;
    char figure;
    bool capture;
    bool white;
} Input;

typedef struct booleans
{
    bool check;
    bool mate;
    bool stalemate;
    bool isWhiteTurn;
    bool white_king_moved;
    bool black_king_moved;
    bool white_rook_A_moved;
    bool white_rook_H_moved;
    bool black_rook_A_moved;
    bool black_rook_H_moved;
    bool playing;
} Booleans;

typedef struct piece
{
    char figure;
} Piece;

typedef struct distances{
    int row_distance;
    bool row_distance_pos;
    int column_distance;
    bool column_distance_pos;
} Distances;

typedef struct game_stats{
    int total_moves;
    int white_captures;
    int black_captures;
    int checks_by_white;
    int checks_by_black;
} GameStats;

Input curr_move(char move[5], char position[8][8]);

void bool_checker(char position[8][8], Booleans* b);
 
bool is_king_in_check(char position[8][8], bool check_WhiteKing);

bool is_move_pattern_valid(Input move, char position[8][8], bool isWhiteTurn, bool modifyBoard, Booleans b);

bool any_valid_moves(char position[8][8], bool isWhiteTurn);

bool pawn_promotion(Input move, char position[8][8]);

void board_print(char position[8][8]);

void reconstruct_move(char board_now[8][8], char board_prev[8][8], char* move_buffer);

void do_promotion(Input move, char position[8][8], char figure);

void format_chess_notation(Input move, char* buffer);