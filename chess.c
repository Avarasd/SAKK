/*
Tóth Avar Áron
DQMKMR
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "chess.h"
#define SMALL_CAP_DISTANCE 32

Piece pawn = {'P'};
Piece rook = {'R'};
Piece knight = {'N'};
Piece bishop = {'B'};
Piece queen = {'Q'};
Piece king = {'K'};

Piece * Pieces[6] = {&pawn, &rook, &knight, &bishop, &queen, &king};

Input curr_move(char move[5], char position[8][8]) {
    Input result;
    result.orig_column = move[0] - 'A';
    result.orig_row = move[1] - '0' - 1;
    result.target_column = move[2] - 'A';
    result.target_row = move[3] - '0' - 1;
    result.figure = position[result.orig_row][result.orig_column];
    if(result.figure > 'A' + SMALL_CAP_DISTANCE){
        result.white = true;
        result.figure -= SMALL_CAP_DISTANCE;
    }else result.white = false;
    result.capture = (position[result.target_row][result.target_column] != '.');
    return result;
}

void setup_distances(Input move, Distances* buffer){
    buffer->row_distance = abs(move.target_row - move.orig_row);
    buffer->row_distance_pos = (move.target_row > move.orig_row);
    buffer->column_distance = abs(move.target_column - move.orig_column);
    buffer->column_distance_pos = (move.target_column > move.orig_column);
}

static void pos_change(Input move, char position[8][8]){
    if(move.white){
        position[move.target_row][move.target_column] = move.figure + SMALL_CAP_DISTANCE;
    }else position[move.target_row][move.target_column] = move.figure;

    position[move.orig_row][move.orig_column] = '.';
}

static bool is_friendly_fire(Input move, char position[8][8]){
    char target_piece = position[move.target_row][move.target_column];
    if(target_piece == '.') return false;

    bool target_piece_white = (target_piece > 'A' + SMALL_CAP_DISTANCE); 
    if(move.white == target_piece_white) return true;
    return false;
}

static bool is_path_free(Input move, char position[8][8], Distances d){
    if (d.row_distance > 0 && d.column_distance == 0) {
        if (d.row_distance_pos) { // Felfelé lép
            for (int i = 1; i < d.row_distance; i++) {
                if (position[move.orig_row + i][move.orig_column] != '.') {
                    return false; // Útban áll valami
                }
            }
        } else { // Lefelé lép
            for (int i = 1; i < d.row_distance; i++) {
                if (position[move.orig_row - i][move.orig_column] != '.') {
                    return false; // Útban áll valami
                }
            }
        }
    }
    // 2. Egyenesen, oszlop mentén (Bástya, resultrálynő)
    else if (d.column_distance > 0 && d.row_distance == 0) {
        if (d.column_distance_pos) { // Jobbra lép
            for (int i = 1; i < d.column_distance; i++) {
                if (position[move.orig_row][move.orig_column + i] != '.') {
                    return false; // Útban áll valami
                }
            }
        } else { // Balra lép
            for (int i = 1; i < d.column_distance; i++) {
                if (position[move.orig_row][move.orig_column - i] != '.') {
                    return false; // Útban áll valami
                }
            }
        }
    }
    // 3. Átlósan (Futó, resultrálynő)
    else if (d.row_distance == d.column_distance && d.row_distance > 0) {
        if (d.row_distance_pos && d.column_distance_pos) { // Fel-jobbra
            for (int i = 1; i < d.row_distance; i++) {
                if (position[move.orig_row + i][move.orig_column + i] != '.') {
                    return false;
                }
            }
        } else if (d.row_distance_pos && !d.column_distance_pos) { // Fel-balra
            for (int i = 1; i < d.row_distance; i++) {
                if (position[move.orig_row + i][move.orig_column - i] != '.') {
                    return false;
                }
            }
        } else if (!d.row_distance_pos && d.column_distance_pos) { // Le-jobbra
            for (int i = 1; i < d.row_distance; i++) {
                if (position[move.orig_row - i][move.orig_column + i] != '.') {
                    return false;
                }
            }
        } else if (!d.row_distance_pos && !d.column_distance_pos) { // Le-balra
            for (int i = 1; i < d.row_distance; i++) {
                if (position[move.orig_row - i][move.orig_column - i] != '.') {
                    return false;
                }
            }
        }
    }

    return true;
}

static bool is_knight_valid(Input move, char position[8][8], Distances d){
    if((d.row_distance == 1 && d.column_distance == 2) || (d.row_distance == 2 && d.column_distance == 1)){
        return true;
    }
    return false;
}

static bool is_rook_valid(Input move, char position[8][8], Distances d){
    if(!((d.row_distance > 0 && d.column_distance == 0 )|| (d.row_distance == 0 && d.column_distance > 0))) return false;

    //Bástya akármennyit nézhet oldalra vagy előre, ezért nem kell azt megnézni, hogy mennyi a távolság

    return is_path_free(move, position, d);
}

static bool is_bishop_valid(Input move, char position[8][8], Distances d){
    if(!(d.row_distance == d.column_distance && d.column_distance != 0)) return false;

    //Futó akármennyit léphet átlósan, nem kell megnézni, hogy mennyi a távolság pontosan.

    return is_path_free(move, position, d);
}

static bool is_pawn_valid(Input move, char position[8][8], Distances d){
    if(!move.capture && position[move.target_row][move.target_column] != '.') return false; //Előre nem üthet a gyalog, csak oldalra.

    if(move.white){

        if(!d.row_distance_pos) return false; //Világos csak előre léphet

        if(!move.capture){

            if(d.column_distance != 0) return false;
        
            if(d.row_distance == 1) return true;

            //Dupla lépés

            if(d.row_distance == 2 && move.orig_row == 1) return (position[move.orig_row + 1][move.orig_column] == '.');
            //Ha dupla lépésnél a köztes mező üres, akkor szabályos a lépés, ha nem üres, akkor nem.
        }else{ //ÜTÉS
            if(d.row_distance == 1 && d.column_distance == 1 ) return true;
        }
    }else{ //SÖTÉT

        if(d.row_distance_pos) return false; //Sötét csak "hátra" léphet

        if(!move.capture){

            if(d.column_distance != 0) return false;

            if(d.row_distance == 1) return true;

            //Dupla lépés

            if(d.row_distance == 2 && move.orig_row == 6) return (position[move.orig_row - 1][move.orig_column] == '.');
            //Ha dupla lépésnél a köztes mező üres, akkor szabályos a lépés, ha nem üres, akkor nem.
            
        }else{ //ÜTÉS
            if(d.row_distance == 1 && d.column_distance == 1) return true;
        }
    }
    //TODO: EN PASSANT

    return false; //Ha semelyik szabályos fajtára nem returnöl a függvény, akkor nem szabályos a lépés.
}

static bool is_king_valid(Input move, char position[8][8], Distances d){
    if(d.row_distance <= 1 && d.column_distance <= 1){
        if(!(d.row_distance != 0 || d.column_distance != 0)){
            return false;
        }
        return true;
    }
    return false;
}

bool is_castling_valid(Input move, char position[8][8], Distances d, Booleans b){
    bool valid_distances = false;
    bool queenside_castle = false;
    if(move.white && b.white_king_moved) return false;
    if(!move.white && b.black_king_moved) return false;
    if(!(d.row_distance == 0 &&  d.column_distance == 2)) return false;
    if(!is_path_free(move, position, d)) return false;
    if(move.white){
        if(move.target_column == 2 && !b.white_rook_A_moved){ 
            valid_distances = true;
            queenside_castle = true;
        }
        if(move.target_column == 6 && !b.white_rook_H_moved) valid_distances = true;
    }else{
        if(move.target_column == 2 && !b.black_rook_A_moved){
            valid_distances = true;
            queenside_castle = true;
        }
        if(move.target_column == 6 && !b.black_rook_H_moved) valid_distances = true;
    }
    if(!valid_distances) return false;
    if(queenside_castle) if(position[move.orig_row][1] != '.') return false;
    char temp_board[8][8];
    memcpy(temp_board, position, sizeof(char[8][8]));
    temp_board[move.orig_row][move.orig_column] = '.';
    for(int i = 0; i < 3; i++){
        temp_board[move.orig_row][move.orig_column + (d.column_distance_pos ? i : -i)] = move.white ? 'k' : 'K';
        if(is_king_in_check(temp_board, move.white)) return false;
        temp_board[move.orig_row][move.orig_column + (d.column_distance_pos ? i : -i)] = '.';
    }
    return true;
}

static bool check_geometry_only(Input move, char position[8][8], Booleans b) {
    Piece movPiece;
    bool Piece_found = false;
    for(int i = 0; i < 6; i++){
        if(move.figure == Pieces[i]->figure){
            movPiece = *Pieces[i];
            Piece_found = true;
        }
    }
    if(!Piece_found) return false; 
    
    if(is_friendly_fire(move, position)) return false; 

    Distances d;
    setup_distances(move, &d);

    switch(movPiece.figure){
        case 'P': return is_pawn_valid(move, position, d);
        case 'N': return is_knight_valid(move, position, d);
        case 'K': return is_king_valid(move, position, d);
        case 'R': return is_rook_valid(move, position, d);
        case 'B': return is_bishop_valid(move, position, d);
        case 'Q': return (is_rook_valid(move, position, d) || is_bishop_valid(move, position, d));
        default: return false;
    }
}

bool is_king_in_check(char position[8][8], bool check_WhiteKing){
    Input attack_input;
    char temp_input[5];
    bool found_king = false;
    for(int row = 0; row < 8; row++){
        for(int column = 0; column < 8; column++){
            if((position[row][column] == 'k' && check_WhiteKing) || (position[row][column] == 'K' && !check_WhiteKing)){
                found_king = true;
                temp_input[2] = column + 'A';
                temp_input[3] = row + '1';
                break;
            }
            if(found_king) break;
        }
    }

    for(int row = 0; row < 8; row++){
        for(int column = 0; column < 8; column++){
            temp_input[0] = column + 'A';
            temp_input[1] = row + '1';
            attack_input = curr_move(temp_input, position);
            Booleans b = {0};
            if(check_geometry_only(attack_input, position, b))return true;
        }
    }
    return false;
}

bool is_move_pattern_valid(Input move, char position[8][8], bool isWhiteTurn, bool modifyBoard, Booleans b){
    if(isWhiteTurn != move.white) return false;
    bool geometry_valid = check_geometry_only(move, position, b);
    if(!geometry_valid){
        Piece movPiece;
        for(int i = 0; i <6; i++) if(move.figure == Pieces[i]->figure)movPiece = *Pieces[i];
        if(movPiece.figure == 'K'){
            Distances d;
            setup_distances(move, &d);

            if(is_castling_valid(move, position, d, b)) geometry_valid = true;
        }
    }
    if(!geometry_valid) return false;
    char temp_board[8][8];
    memcpy(temp_board, position, sizeof(char[8][8]));    
    pos_change(move, temp_board);

    if(is_king_in_check(temp_board, isWhiteTurn)) return false;

    if(modifyBoard){
        Distances d;
        setup_distances(move, &d);
        if(move.figure == 'K' && d.column_distance == 2){
            int r = move.orig_row;
            bool short_castle = d.column_distance_pos;
            if(short_castle){
                position[r][5] = position[r][7];
                position[r][7] = '.';
            }else{
                position[r][3] = position[r][0];
                position[r][0] = '.';
            }
        }
        pos_change(move, position);
    }
    return true;
}

void board_print(char position[8][8]){
    for (int sor = 7; sor >= 0; sor--) {
                for (int elem = 0; elem < 8; elem++) {
                    printf("%c ", position[sor][elem]);
                }
                printf("%d ",sor + 1);
                printf(" \n");
            }
            printf("A B C D E F G H \n\n");
}

void bool_checker(char position[8][8], Booleans* b){
    if(position[0][0] != 'r') b->white_rook_A_moved = true;
    if(position[0][7] != 'r') b->white_rook_H_moved = true;
    if(position[7][0] != 'R') b->black_rook_A_moved = true;
    if(position[7][7] != 'R') b->black_rook_H_moved = true;
    if(position[0][4] != 'k') b->white_king_moved = true;
    if(position[7][4] != 'K') b->black_king_moved = true;
}

bool any_valid_moves(char position[8][8], bool isWhiteTurn){
    Input test_move;
    Booleans b = {0};
    for(int r1 = 0; r1 < 8; r1++){

        for(int c1 = 0; c1 < 8; c1++){

            char p = position[r1][c1];
            if(p == '.') continue;
            bool isPieceWhite = (p > 'A' + SMALL_CAP_DISTANCE);
            if(isPieceWhite != isWhiteTurn) continue;

            for(int r2 = 0; r2 < 8; r2++){

                for(int c2 = 0; c2 < 8; c2++){

                    if(r1 == r2 && c1 == c2) continue;
                    test_move.orig_row = r1;
                    test_move.orig_column = c1;
                    test_move.target_row = r2;
                    test_move.target_column = c2;
                    test_move.figure = (p > 'A' + SMALL_CAP_DISTANCE) ? p- SMALL_CAP_DISTANCE : p;
                    test_move.white = isPieceWhite;
                    test_move.capture = (position[r2][c2] != '.');

                    if(is_move_pattern_valid(test_move, position, isWhiteTurn, false, b)) return true;
                }
            }
        }
    }
    return false;
}

bool pawn_promotion(Input move, char position[8][8]){
    if(move.figure == 'P' && ((move.white && move.target_row == 7) || (!move.white && move.target_row == 0))) return true;
    else return false;

}

void do_promotion(Input move, char position[8][8], char figure){
    move.figure = figure;
    pos_change(move, position);
}

void reconstruct_move(char board_now[8][8], char board_prev[8][8], char* move_buffer){
    int changes = 0;
    int src_r = -1, src_c = -1;
    int dst_r = -1, dst_c = -1;
    for(int row = 0; row < 8; row++){
        for(int column = 0; column < 8; column++){
            if(board_now[row][column] != board_prev[row][column]){
                changes++;

                if(board_now[row][column] == '.'){//Innen ellépett
                    char figure_prev = board_prev[row][column];
                    if(figure_prev == 'k' || figure_prev == 'K'){
                        src_r = row;
                        src_c = column;
                    } else if(src_r == -1){
                        src_r = row;
                        src_c = column;
                    }
                } else { //Ide lépett
                    char figure_now = board_now[row][column];
                    if(figure_now == 'k' || figure_now == 'K'){
                        dst_r = row;
                        dst_c = column;
                    } else if (dst_r == -1) {
                        dst_r = row; dst_c = column;
                    }
                }
            }
        }
    }
    if(src_r != -1 && dst_r != -1 && src_c != -1 && dst_c != -1){
        move_buffer[0] = src_c + 'A';
        move_buffer[1] = src_r + '1';
        move_buffer[2] = dst_c + 'A';
        move_buffer[3] = dst_r + '1';
        move_buffer[4] = '\0';
    }
}

void format_chess_notation(Input move, char* buffer){
    if (move.figure == 'K' && abs(move.target_column - move.orig_column) == 2) {
        if (move.target_column > move.orig_column) {
            strcpy(buffer, "O-O");// Rövid sánc
        } else {
            strcpy(buffer, "O-O-O");// Hosszú sánc
        }
        return;
    }

    char separator = move.capture ? 'x' : '-';
    
    if (move.figure == 'P') {
        sprintf(buffer, "%c%d%c%c%d",
            move.orig_column + 'a',
            move.orig_row + 1,
            separator,
            move.target_column + 'a',
            move.target_row + 1);
    } 
    else {
        sprintf(buffer, "%c%c%d%c%c%d", 
            move.figure,
            move.orig_column + 'a',
            move.orig_row + 1,
            separator,
            move.target_column + 'a',
            move.target_row + 1);
    }
}

int check_eval(char board[8][8]) {
    int eval = 0;
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            char piece = board[i][j];
            if(piece == '.') continue;
            
            int value = 0;
            bool isWhite = false;
            
            if(piece > 'Z') { // Lowercase -> White
                isWhite = true;
                piece -= 32; // Convert to uppercase for switch statement
            } else { // Uppercase -> Black
                isWhite = false;
            }
            
            switch(piece){
                case 'Q': value = VAL_QUEEN; break;
                case 'R': value = VAL_ROOK; break;
                case 'N': value = VAL_KNIGHT; break;
                case 'B': value = VAL_BISHOP; break;
                case 'P': value = VAL_PAWN; break;
                case 'K': value = VAL_KING; break;
                default: value = 0; break;
            }
            
            if(isWhite) eval += value;
            else eval -= value;
        }
    }
    return eval;
}