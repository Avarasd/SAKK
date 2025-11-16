#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "chess.h"
#define SMALL_CAP_DISTANCE 32

Piece pawn = {'P', 1, 0, 0, false, true};
Piece rook = {'R', 7, 7, 0, true, true};
Piece knight = {'N', 2, 2, 0, true, true};
Piece bishop = {'B', 0, 0, 7, true, true};
Piece queen = {'Q', 7, 7, 7, true, true};
Piece king = {'K', 1, 1, 1, true, true};

Piece * Pieces[6] = {&pawn, &rook, &knight, &bishop, &queen, &king};

Input curr_move(char move[5], char position[8][8]) {
    Input result;
    result.orig_column = move[0] - 'A';
    result.orig_row = move[1] - '0' - 1;
    result.target_column = move[2] - 'A';
    result.target_row = move[3] - '0' - 1;
    result.figure = position[result.orig_row][result.orig_column];
    if(result.figure > 96){
        result.white = true;
        result.figure -= SMALL_CAP_DISTANCE;
    }else result.white = false;
    result.capture = (position[result.target_row][result.target_column] != '.');
    return result;
};
static void pos_change(Input move, char position[8][8]){
    if(move.white){
        position[move.target_row][move.target_column] = move.figure + SMALL_CAP_DISTANCE;
    }else position[move.target_row][move.target_column] = move.figure;

    position[move.orig_row][move.orig_column] = '.';
}

static bool is_friendly_fire(Input move, char position[8][8]){
    char target_piece = position[move.target_row][move.target_column];
    if(target_piece == '.') return false;

    bool target_piece_white = (target_piece > 96); 
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

    //TODO SÁNCOLÁSS

    return false;
}

bool is_move_pattern_valid(Input move, char position[8][8]){
    Piece lepoPiece;
    bool Piece_talalt = false;
    for(int i = 0; i < 6; i++){
        if(move.figure == Pieces[i]->figure){
            lepoPiece = *Pieces[i];
            Piece_talalt = true;
        }
    }

    if(!Piece_talalt) return false; //Na nem sikerült megtalálni a bábut, azaz rossz az Input, akkor false return
    if(is_friendly_fire(move, position)) return false; //Ha a játékos egy saját bábuját próbálja leütni.

    Distances d;
    d.row_distance = abs(move.target_row - move.orig_row);
    d.row_distance_pos = (move.target_row > move.orig_row);
    d.column_distance = abs(move.target_column - move.orig_column);
    d.column_distance_pos = (move.target_column > move.orig_column);
    bool lephet_e;

    switch(lepoPiece.figure){
        case 'P':
            lephet_e = is_pawn_valid(move, position, d);
            break;
        case 'N':
            lephet_e = is_knight_valid(move, position, d);
            break;
        case 'K':
            lephet_e = is_king_valid(move, position, d);
            break;
        case 'R':
            lephet_e = is_rook_valid(move, position, d);
            break;
        case 'B':
            lephet_e = is_bishop_valid(move, position, d);
            break;
        case 'Q':
            lephet_e = (is_rook_valid(move, position, d) || is_bishop_valid(move, position, d));
            //A resultrálynő szabályosan úgy léphet, mint a futó és a bástya.
            break;
        default:
            lephet_e = false; //Ha valahogyan olyan figure került a változóba, ami nem létezik, akkor egyből false, valamilyen hiba adódott.
            break;
    }
    
    if(lephet_e){
        pos_change(move, position);
        return true;
    }

    return false;
}

//TODO: SAKKHELYZET FELISMERÉSE; JÁTÉKOSOK VÁLTÁSA (jelenleg akármennyit lehet pl. fehérrel lépni egymás után);