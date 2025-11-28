#include <stdbool.h>

typedef enum{
    STATE_MAIN_MENU,
    STATE_GAME_MENU,
    STATE_GAME_RUNNING,
    STATE_GAME_END,
    STATE_ANALYSIS_MENU,
    STATE_ANALYSIS_SETUP,
    STATE_ANALYSIS_RUNNING,
    STATE_ANALYSIS_END,
    STATE_EXIT
} State;

typedef enum{
    COORD_BOARD_X = 32,
    COORD_BOARD_Y = 5,
    COORD_INFO_X = 75,
    COORD_INFO_Y = 5,
    COORD_INPUT_X = 31,
    COORD_INPUT_Y = 17,
    COORD_MENU_X = 31,
    COORD_MENU_Y = 5
} Coords;

void draw_square(int x1, int y1, int x2, int y2);
void display_init();
void display_clear();
void display_sleep(int seconds);

void display_all_alternative_moves(char (*move_arr)[5], int size);
void display_board(char position[8][8], int x, int y);
void display_info(bool isWhiteTurn, int move, int eval);
void display_get_input(char* input);
void display_game_state(bool isValid, bool isCheck, bool isMate, bool isStalemate);
void update_moves(char* move, int move_count);
int display_get_char(void);
int display_get_branch(void);
char display_ask_promotion(void);

State game_mode_menu(void);
State display_menu(void);
State analysis_mode_menu(void);

bool analysis_mode_file_set(char* filename);
int display_analysis_info(int eval);
State game_mode_end(char* buffer);