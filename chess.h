/*
Tóth Avar Áron
DQMKMR
*/

#define SMALL_CAP_DISTANCE 32

#define VAL_QUEEN 9
#define VAL_ROOK 5
#define VAL_KNIGHT 3
#define VAL_BISHOP 3
#define VAL_PAWN 1
#define VAL_KING 0

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

Input curr_move(char move[5], char position[8][8]);

void bool_checker(char position[8][8], Booleans* b);
 
bool is_king_in_check(char position[8][8], bool check_WhiteKing);

bool is_move_pattern_valid(Input move, char position[8][8], bool isWhiteTurn, bool modifyBoard, Booleans b, int en_passant_col, int* next_en_passant_col);

bool any_valid_moves(char position[8][8], bool isWhiteTurn, int en_passant_col);

bool pawn_promotion(Input move);

void reconstruct_move(char board_now[8][8], char board_prev[8][8], char* move_buffer);

void do_promotion(Input move, char position[8][8], char figure);

void format_chess_notation(Input move, char* buffer);

int check_eval(char board[8][8]);