#include <stdio.h>
#include <time.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include "econio.h"
#include "display.h"
#ifdef _WIN32
#include <windows.h>
#endif

void display_init(void) {
    #ifdef _WIN32
    SetConsoleOutputCP(65001);
    #endif
    econio_rawmode();
}

void display_clear(){
    econio_clrscr();
}

void display_sleep(int seconds){
    econio_sleep(seconds);
}

//FORRÁS: INTERNET
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

int display_get_char(void){
    int input = -1;
    econio_gotoxy(31, 17);
    printf("Adj meg egy lépésszámot:");
    draw_square(30, 16, 62, 18);
    econio_gotoxy(55, 17);
    econio_normalmode();
    scanf(" %d", &input);
    econio_rawmode();
    econio_gotoxy(55,17);
    for(int i = 0; i < 5; i++) printf(" ");
    return input;
}

int display_get_branch(void){
    int input = -1;
    econio_gotoxy(31, 17);
    printf("Válassz sorszámot:");
    draw_square(30, 16, 62, 18);
    econio_gotoxy(55, 17);
    econio_normalmode();
    scanf(" %d", &input);
    econio_rawmode();
    econio_gotoxy(55,17);
    for(int i = 0; i < 5; i++) printf(" ");
    return input;
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

void display_info(bool isWhiteTurn, int move){
    econio_textcolor(COL_WHITE);
    econio_gotoxy(75, 5);
    printf("                        ");
    econio_gotoxy(75, 5);
    printf("%d. LÉPÉS, %s JÖN", move, isWhiteTurn ? "VILÁGOS" : "SÖTÉT");
}

void display_game_state(bool isValid, bool isCheck, bool isMate, bool isStalemate){
    //ELŐZŐ TÖRLÉSE
    econio_gotoxy(75, 7);
    for(int i=0; i< 30; i++) printf(" ");

    econio_textcolor(COL_RED);
    econio_textbackground(COL_BLUE);
    econio_gotoxy(75, 7);
    if(isValid){
    if(isMate) printf("MATT! A játéknak vége.");
    else if(isStalemate) printf("PATT! A játéknak vége.");
    else if(isCheck) printf("SAKK!");
    else printf("Szabályos lépés!");
    } else{
        printf("Szabálytalan lépés!");
    }
    econio_textcolor(COL_RESET);
    econio_textbackground(COL_RESET);
}

void display_get_input(char* input){
    econio_gotoxy(31, 17);
    printf("Adj meg egy lépést:");
    draw_square(30, 16, 62, 18);
    econio_gotoxy(55,17);
    for(int i = 0; i < 5; i++) printf(" ");
    econio_gotoxy(55, 17);
    econio_normalmode();
    scanf("%49s", input);
    econio_rawmode();
}

void update_moves(char* move, int move_count){
    int current_row = 5 + ((move_count - 1)/2);
    
    econio_gotoxy(6, 3);
    printf("Lépések:");

    if(move_count %2 != 0){
        // New row (White move)
        // Clear the new row (which was the bottom border of the previous row)
        econio_gotoxy(1, current_row);
        printf("                             "); 
        
        // Print move number and move
        econio_gotoxy(2, current_row);
        printf("%d.",move_count/2 + 1);
        econio_gotoxy(6, current_row);
        printf("%-10s", move);
        
        // Clear the line where the new bottom will be
        econio_gotoxy(1, current_row + 1);
        printf("                             ");
    } else {
        // Same row (Black move)
        // Just print the move
        econio_gotoxy(18, current_row);
        printf("%-10s", move);
    }
    // Always draw the box to ensure the bottom border is present
    draw_square(1, 4, 28, current_row + 1);
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

    while(getchar() != '\n');
    econio_rawmode();
    return minutes;
}

State game_mode_end(char* buffer){
    char input[50];
    econio_clrscr();
    econio_gotoxy(34, 5);
    printf("Add meg a parti nevét (max 20 karakter)");
    draw_square(29, 4, 90, 9);

    econio_gotoxy(34, 7);
    econio_normalmode();
    scanf("%49s", input);
    sprintf(buffer, "Games\\%s.dat", input);
    while(getchar() != '\n');
    econio_rawmode();
    econio_gotoxy(34, 8);
    printf("Sikeres mentés! Nyomj egy gombot a folytatáshoz");
    while(true){
        if(econio_kbhit()) return STATE_MAIN_MENU;
    }
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
            else return STATE_GAME_RUNNING;
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

//FORRÁS: INTERNET
bool anal_mode_file_set(char* filename){
    #ifdef _WIN32
    WIN32_FIND_DATA findData;
    HANDLE hFind;
    econio_clrscr();
    char fileList[100][40];
    int totalFiles = 0;

    hFind = FindFirstFile("Games\\*.dat", &findData);
    if(hFind == INVALID_HANDLE_VALUE){
        econio_gotoxy(31, 3);
        printf("Nincs betölthető fájl");
        econio_gotoxy(31, 5);
        printf("Nyomj egy gombot a visszalépéshez");
        draw_square(29,2,67,7);
        while(true){
            if(econio_kbhit()) return false;
        }
    }
    econio_gotoxy(32, 3);
    printf("Válassz sorszámot:");
    do{
        if(totalFiles < 100){
            // econio_gotoxy(31, 9 + y_offset);
            strcpy(fileList[totalFiles], findData.cFileName);
            // printf("%d. %s", totalFiles + 1, fileList[totalFiles]);
            totalFiles++;
            // y_offset += 2;
        }
    } while(FindNextFile(hFind, &findData) != 0);
    FindClose(hFind);

    int page = 0;
    int totalPages = (totalFiles + 9) / 10;
    while(true){
        if(page < 0) page = 0;
        if(page >= totalPages) page = totalPages - 1;
        if(totalFiles == 0) return false; 

        econio_clrscr();
        econio_gotoxy(31, 1);
        printf("<Válassz egy fájlt> Oldal: %d/%d", page + 1, totalPages);
        int start = page * 10;
        int end = start + 10;
        if(end > totalFiles) end = totalFiles;
        int itemsOnPage = end - start;
        int i = 0;
        for(i = start; i < end; i++){
            econio_gotoxy(31, 3 + (i%10)*2);
            int displayNum = (i % 10) + 1;
            if (displayNum == 10) displayNum = 0;
            
            printf("%d. %.32s", displayNum, fileList[i]);
        }
        draw_square(29,0,67,5 + itemsOnPage*2);

        econio_gotoxy(75, 1);
        printf("Az alábbi opciók közül válassz:");
        econio_gotoxy(75, 3);
        printf("< Előző lapra lépés");
        econio_gotoxy(75, 5);
        printf("> Következő lapra lépés");
        econio_gotoxy(75, 7);
        printf("0-9: Fájl kiválasztása");
        econio_gotoxy(75, 9);
        printf("X: kilépés");

        econio_gotoxy(32, 7 + itemsOnPage*2);
        while(!econio_kbhit());
        char input = econio_getch();

        switch(input){
            case KEY_LEFT: 
                page--; 
                break;
            case KEY_RIGHT: 
                page++; 
                break;
            case KEY_ESCAPE:
            case 'x':
            case 'b':
                return false;

            default: {
                if(input >= '0' && input <= '9'){
                    int selection = input - '0';
                    if(selection == 0) selection = 10;
                    if(selection <= itemsOnPage){
                        sprintf(filename, "Games\\%s", fileList[start + selection - 1]);
                        return true;
                    }
                } else {
                    econio_gotoxy(32, 9 + itemsOnPage*2);
                    printf("Érvénytelen sorszám");
                    econio_sleep(1);
                }
            }
            break;
        }
    }
    #else
    econio_gotoxy(31, 5);
    printf("Ez a funkció sajnos csak Windowson elérhető");
    return false;
    #endif
}

int display_anal_info(void){
    econio_textcolor(COL_WHITE);
    econio_gotoxy(75, 4);
    printf("                               ");
    econio_gotoxy(75, 4);
    printf("Válassz az alábbi opciók közül!");
    econio_gotoxy(75, 7);
    printf("< Hátra lépés");
    econio_gotoxy(75, 8);
    printf("> Előre lépés");
    econio_gotoxy(75, 9);
    printf("1. Alapállásra lépés");
    econio_gotoxy(75, 10);
    printf("2. Végállásra lépés");
    econio_gotoxy(75, 11);
    printf("3. Valahányadik lépésre lépés");
    econio_gotoxy(75, 12);
    printf("4. Új mellékág létrehozása");
    econio_gotoxy(75, 13);
    printf("5. Mellékágra lépés");
    econio_gotoxy(75, 14);
    printf("X. Kilépés");
    draw_square(74, 6, 104, 15);
    while(true){
        if(econio_kbhit()){
            switch(econio_getch()){
                case KEY_LEFT : return 'l';break;
                case KEY_RIGHT : return 'r';break;
                case 'x' : return 'x';break;
                case 'X' : return 'x';break;
                case '1' : return '1'; break;
                case '2' : return '2'; break;
                case '3' : return '3'; break;
                case '4' : return '4'; break;
                case '5' : return '5'; break;
            }
        }
    }
}

void display_all_alternative_moves(char (*move_arr)[5], int size){
    for(int y = 39; y <= 70; y++){
        econio_gotoxy(20, y);
        printf("                                    ");
    }
    econio_textcolor(COL_RED);
    econio_gotoxy(40, 21);
    if(size == 0){
        printf("Nincs alternatív lépés");
        econio_textcolor(COL_RESET);
        return;
    }
    else printf("Összes elérhető lépés:");
    for(int i = 0; i < size; i++){
        int column = i%3;
        int row = i/3;

        econio_gotoxy(40 + column*10, 23 + row);
        printf("%d. %.5s", i + 1, move_arr[i]);
    }
    econio_gotoxy(60, 20);
    draw_square(39, 20, 70, 23 + (size+2)/3);
    econio_textcolor(COL_RESET);
}

char display_ask_promotion(void){
    econio_gotoxy(75, 7);
    printf("GYALOGÁTVÁLTOZÁS!");
    econio_gotoxy(75, 9);
    printf("Add meg a bábut, amivé változzon: Q R N B");
    econio_gotoxy(55,17);

    while(true){
        if(econio_kbhit()){
            char input = econio_getch();
            char result = 0;
            econio_gotoxy(75, 7);
            switch(input){
                case 'q': case 'Q': result = 'Q'; break;
                case 'r': case 'R': result = 'R'; break;
                case 'n': case 'N': result = 'N'; break;
                case 'b': case 'B': result = 'B'; break;
            }
            if(result != 0){
                econio_gotoxy(75, 7);
                printf("                  ");  
                econio_gotoxy(75, 9);
                printf("                                         ");
                return result; 
            }
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
                    case 'X': return STATE_EXIT; break;
                }
            }
        }
    }
}