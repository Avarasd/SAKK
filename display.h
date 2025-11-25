typedef enum{
    STATE_MAIN_MENU,
    STATE_GAME_MENU,
    STATE_GAME_SETUP,
    STATE_GAME_RUNNING,
    STATE_GAME_END,
    STATE_ANALYSIS_MENU,
    STATE_ANALYSIS_SETUP,
    STATE_ANALYSIS_RUNNING,
    STATE_EXIT
} State;

void display_init();

void game_mode_display(int minutes);

int game_mode_time_set(void);

State game_mode_menu(void);

State anal_mode_menu(void);

bool anal_mode_file_set(char* filename);

void anal_mode_display(char fileName[50]);

State display_menu(void);