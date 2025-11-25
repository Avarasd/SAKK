#include <stdio.h>
#include <time.h>      // REQUIRED for time()
#include <stdbool.h>
#include <stdlib.h>
#include "econio.h"
#ifdef _WIN32
#include <windows.h> // Ez kell a SetConsoleOutputCP-hez
#endif
extern char board[8][8];

typedef enum{
    STATE_MAIN_MENU,
    STATE_GAME_MENU,
    STATE_GAME_SETUP,
    STATE_GAME_RUNNING,
    STATE_ANALYSIS_MENU,
    STATE_ANALYSIS_SETUP,
    STATE_ANALYSIS_RUNNING,
    STATE_EXIT
} State;

void display_init() {
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    #endif
    econio_rawmode(); // Alapból raw mód a menühöz
}

void game_init(void){
    int minutes = 1;
}

void draw_square(int x1, int y1, int x2, int y2) {
    econio_gotoxy(x1,y1); putchar('+');
    econio_gotoxy(x2,y1); putchar('+');
    econio_gotoxy(x1,y2); putchar('+');
    econio_gotoxy(x2,y2); putchar('+');
    for (int i=x1+1; i < x2; i++)
    {
        econio_gotoxy(i,y1); putchar('-');
        econio_gotoxy(i,y2); putchar('-');
    }
    for (int i=y1+1; i < y2; i++)
    {
        econio_gotoxy(x1,i); putchar('|');
        econio_gotoxy(x2,i); putchar('|');
    }
}

 void display_board(char position[8][8], int x, int y){
    econio_textcolor(COL_CYAN);
    econio_gotoxy(x + 3, y);
    printf(" A  B  C  D  E  F  G  H");
    
    for(int row = 7; row >= 0; row--){
        econio_gotoxy(x, y + 1 + (7 - row));
        printf("%d", row + 1);
        for(int column = 0; column < 8; column++){
            econio_gotoxy(x + 3 + (column * 3), y + 1 + (7 - row));

            char curr_piece = position[row][column];

            bool black_square = (row + column) % 2 == 0;
            econio_textbackground(black_square ? COL_BLACK : COL_DARKGRAY);

            if(curr_piece == '.'){
            printf("   ");
            }else{
                if(curr_piece > 96){
                econio_textcolor(COL_YELLOW);
            }
            else econio_textcolor(COL_LIGHTGRAY);
            printf(" %c ", curr_piece);
            }
        }
        econio_gotoxy(x + 3 + (8 * 3) + 1, y + 1 + (7 - row));
        econio_textbackground(COL_RESET);
        econio_textcolor(COL_CYAN);
        printf("%d", row + 1);
    }
    econio_textcolor(COL_CYAN);
    econio_gotoxy(x + 3, y + 10);
    printf(" A  B  C  D  E  F  G  H");

    econio_textcolor(COL_RESET);
    econio_textbackground(COL_RESET);
}

void econio_display_info(bool isWhiteTurn, int move){
    econio_textcolor(COL_WHITE);
    econio_gotoxy(60, 5);
    printf("%d. LÉPÉS, %s JÖN", move, isWhiteTurn ? "VILÁGOS" : "SÖTÉT");
}

void display_get_input(char* input){
    econio_normalmode();
    econio_gotoxy(30, 17);
    printf("Lepes:");
    for(int i = 0; i < 20; i++) printf(" ");
    econio_gotoxy(47, 17);
    scanf("%s", &input);
    econio_rawmode();
}

void display_clear(){
    econio_clrscr();
}

void game_mode_display(int minutes){
    econio_clrscr();
    printf("%d", minutes);
    econio_sleep(10);
    //display_board(board, 30, 2);
}

int game_mode_time_set(void){
    int minutes;
    econio_clrscr();
    econio_gotoxy(32, 5);
    printf("<<Add meg a percek számát!>>");
    draw_square(29,4,60, 9);
    econio_gotoxy(34, 7);
    econio_normalmode();
    scanf(" %d", &minutes);
    econio_rawmode();
    return minutes;
}

State game_mode_menu(void){
    econio_clrscr();
    econio_gotoxy(34, 5);
    printf("<<Üdvözöllek a játék módban>>");
    econio_gotoxy(31, 7);
    printf("Nyomj meg egy gombot a kezdéshez!");
    econio_gotoxy(31, 9);
    printf("Vagy nyomd meg a B-t visszalépéshez!");
    draw_square(29,4,67,11);
    econio_rawmode();
    while(true){
        if(econio_kbhit()){
            char input = econio_getch();
            if(input == 'b' || input == 'B') return STATE_MAIN_MENU;
            else return STATE_GAME_SETUP;
        }
    }
    //econio_draw_board();
}
State anal_mode_menu(void){
    econio_clrscr();
    econio_gotoxy(34, 5);
    printf("<<Üdvözöllek az elemző módban>>");
    econio_gotoxy(31, 7);
    printf("Nyomj meg egy gombot a kezdéshez!");
    econio_gotoxy(31, 9);
    printf("Vagy nyomd meg a B-t visszalépéshez!");
    draw_square(29,4,67,11);
    econio_rawmode();
    while(true){
        if(econio_kbhit()){
            char input = econio_getch();
            if(input == 'b' || input == 'B') return STATE_MAIN_MENU;
            else return STATE_ANALYSIS_SETUP;
        }
    }
}

State display_menu(void){
    while(true){
        econio_clrscr();
        econio_gotoxy(31, 5); printf("<<Üdvözöllek a Sakk programomban!>>");
        econio_gotoxy(32, 7); printf("Válassz az alábbi menüpontok közül:");
        econio_gotoxy(30, 8); printf("1. Játék indítása két játékos között");
        econio_gotoxy(30, 9); printf("2. Elemző mód indítása");
        econio_gotoxy(30, 10); printf("X. Bezárás");
        draw_square(29,4,67,11);
        econio_rawmode();
        while(true){
            if(econio_kbhit()){
                char input = econio_getch();
                switch(input){
                    case '1': return STATE_GAME_MENU; break;
                    case '2': return STATE_ANALYSIS_MENU; break;
                    case 'x': return STATE_EXIT; break;
                }
            }
        }
    }
}

int main() {
    display_init();
    State currentState = STATE_MAIN_MENU;
    int minutes = 0;
    while(currentState != STATE_EXIT){
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
                game_mode_display(minutes);
            case STATE_ANALYSIS_RUNNING:
                game_init();
        }
    }
    // int input = display_menu();
    // switch(input){
    //     case 1: game_mode_menu(); break;
    //     case 2: anal_mode_menu(); break;
    //     case 3: exit; break;
    // }
}
