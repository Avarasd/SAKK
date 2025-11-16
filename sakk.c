#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "Sakk.h"
#define SMALL_CAP_DISTANCE 32

babu gyalogos = {'P', 1, 0, 0, false, true};
babu bastya = {'R', 7, 7, 0, true, true};
babu huszar = {'N', 2, 2, 0, true, true};
babu futo = {'B', 0, 0, 7, true, true};
babu kiralyno = {'Q', 7, 7, 7, true, true};
babu kiraly = {'K', 1, 1, 1, true, true};

babu * babuk[6] = {&gyalogos, &bastya, &huszar, &futo, &kiralyno, &kiraly};

input mostlepes(char lepes[5], char allas[8][8]) {
    input ki;
    ki.honnanoszlop = lepes[0] - 'A';
    printf("%d", ki.honnanoszlop);
    ki.honnansor = lepes[1] - '0' - 1;
    printf("%d", ki.honnansor);
    ki.hovaoszlop = lepes[2] - 'A';
    printf("%d", ki.hovaoszlop);
    ki.hovasor = lepes[3] - '0' - 1;
    printf("%d", ki.hovasor);
    ki.figura = allas[ki.honnansor][ki.honnanoszlop];
    if(ki.figura > 96){
        ki.vilagos = true;
        ki.figura -= SMALL_CAP_DISTANCE;
    }else ki.vilagos = false;
    printf("%c \n", allas[ki.honnansor][ki.honnanoszlop]);
    printf("ELSO CHECKPOINT %c \n", ki.figura);
    ki.utes = (allas[ki.hovasor][ki.hovaoszlop] != '.');
    printf("%d\n", ki.utes);
    return ki;
};
static void allasmegvaltoztatas(input lepes, char allas[8][8]){
    if(lepes.vilagos){
        allas[lepes.hovasor][lepes.hovaoszlop] = lepes.figura + SMALL_CAP_DISTANCE;
    }else allas[lepes.hovasor][lepes.hovaoszlop] = lepes.figura;

    allas[lepes.honnansor][lepes.honnanoszlop] = '.';
}

static bool celmezo_sajat_e(input lepes, char allas[8][8]){
    char celbabu = allas[lepes.hovasor][lepes.hovaoszlop];
    if(celbabu == '.')return false;

    bool celbabu_vilagos = (celbabu > 96); 
    if(lepes.vilagos == celbabu_vilagos) return true;
    return false;
}

static bool ut_szabad_e(input lepes, char allas[8][8], tavolsagok t){
    if (t.tavolsagsor > 0 && t.tavolsagoszlop == 0) {
        if (t.sortavpozitiv) { // Felfelé lép
            for (int i = 1; i < t.tavolsagsor; i++) {
                if (allas[lepes.honnansor + i][lepes.honnanoszlop] != '.') {
                    return false; // Útban áll valami
                }
            }
        } else { // Lefelé lép
            for (int i = 1; i < t.tavolsagsor; i++) {
                if (allas[lepes.honnansor - i][lepes.honnanoszlop] != '.') {
                    return false; // Útban áll valami
                }
            }
        }
    }
    // 2. Egyenesen, oszlop mentén (Bástya, Királynő)
    else if (t.tavolsagoszlop > 0 && t.tavolsagsor == 0) {
        if (t.oszloptavpozitiv) { // Jobbra lép
            for (int i = 1; i < t.tavolsagoszlop; i++) {
                if (allas[lepes.honnansor][lepes.honnanoszlop + i] != '.') {
                    return false; // Útban áll valami
                }
            }
        } else { // Balra lép
            for (int i = 1; i < t.tavolsagoszlop; i++) {
                if (allas[lepes.honnansor][lepes.honnanoszlop - i] != '.') {
                    return false; // Útban áll valami
                }
            }
        }
    }
    // 3. Átlósan (Futó, Királynő)
    else if (t.tavolsagsor == t.tavolsagoszlop && t.tavolsagsor > 0) {
        if (t.sortavpozitiv && t.oszloptavpozitiv) { // Fel-jobbra
            for (int i = 1; i < t.tavolsagsor; i++) {
                if (allas[lepes.honnansor + i][lepes.honnanoszlop + i] != '.') {
                    return false;
                }
            }
        } else if (t.sortavpozitiv && !t.oszloptavpozitiv) { // Fel-balra
            for (int i = 1; i < t.tavolsagsor; i++) {
                if (allas[lepes.honnansor + i][lepes.honnanoszlop - i] != '.') {
                    return false;
                }
            }
        } else if (!t.sortavpozitiv && t.oszloptavpozitiv) { // Le-jobbra
            for (int i = 1; i < t.tavolsagsor; i++) {
                if (allas[lepes.honnansor - i][lepes.honnanoszlop + i] != '.') {
                    return false;
                }
            }
        } else if (!t.sortavpozitiv && !t.oszloptavpozitiv) { // Le-balra
            for (int i = 1; i < t.tavolsagsor; i++) {
                if (allas[lepes.honnansor - i][lepes.honnanoszlop - i] != '.') {
                    return false;
                }
            }
        }
    }

    return true;
}

static bool huszar_szabalyos_e(input lepes, char allas[8][8], tavolsagok t){
    if((t.tavolsagsor == 1 && t.tavolsagoszlop == 2) || (t.tavolsagsor == 2 && t.tavolsagoszlop == 1)){
        return true;
    }
    return false;
}

static bool bastya_szabalyos_e(input lepes, char allas[8][8], tavolsagok t){
    if(!(t.tavolsagsor > 0 && t.tavolsagoszlop == 0 )|| (t.tavolsagsor == 0 && t.tavolsagoszlop > 0)) return false;

    //Bástya akármennyit nézhet oldalra vagy előre, ezért nem kell azt megnézni, hogy mennyi a távolság

    return ut_szabad_e(lepes, allas, t);
}

static bool futo_szabalyos_e(input lepes, char allas[8][8], tavolsagok t){
    if(!(t.tavolsagsor == t.tavolsagoszlop && t.tavolsagoszlop != 0)) return false;

    //Futó akármennyit léphet átlósan, nem kell megnézni, hogy mennyi a távolság pontosan.

    return ut_szabad_e(lepes, allas, t);
}

static bool gyalog_szabalyos_e(input lepes, char allas[8][8], tavolsagok t){
    if(!lepes.utes && allas[lepes.hovasor][lepes.hovaoszlop] != '.') return false; //Előre nem üthet a gyalog, csak oldalra.

    if(lepes.vilagos){

        if(!t.sortavpozitiv) return false; //Világos csak előre léphet

        if(!lepes.utes){

            if(t.tavolsagoszlop != 0) return false;
        
            if(t.tavolsagsor == 1) return true;

            //Dupla lépés

            if(t.tavolsagsor == 2 && lepes.honnansor == 1) return (allas[lepes.honnansor + 1][lepes.honnanoszlop] == '.');
            //Ha dupla lépésnél a köztes mező üres, akkor szabályos a lépés, ha nem üres, akkor nem.
        }else{ //ÜTÉS
            if(t.tavolsagsor == 1 && t.tavolsagoszlop == 1 ) return true;
        }
    }else{ //SÖTÉT

        if(t.sortavpozitiv) return false; //Sötét csak "hátra" léphet

        if(!lepes.utes){

            if(t.tavolsagoszlop != 0) return false;

            if(t.tavolsagsor == 1) return true;

            //Dupla lépés

            if(t.tavolsagsor == 2 && lepes.honnansor == 6) return (allas[lepes.honnansor - 1][lepes.honnanoszlop] == '.');
            //Ha dupla lépésnél a köztes mező üres, akkor szabályos a lépés, ha nem üres, akkor nem.
            
        }else{ //ÜTÉS
            if(t.tavolsagsor == 1 && t.tavolsagoszlop == 1) return true;
        }
    }
    //TODO: EN PASSANT

    return false; //Ha semelyik szabályos fajtára nem returnöl a függvény, akkor nem szabályos a lépés.
}

static bool kiraly_szabalyos_e(input lepes, char allas[8][8], tavolsagok t){
    if(t.tavolsagsor <= 1 && t.tavolsagoszlop <= 1){
        if(!(t.tavolsagsor != 0 || t.tavolsagoszlop != 0)){
            return false;
        }
        return true;
    }

    //TODO SÁNCOLÁSS

    return false;
}

bool szabalyos_lepesforma_e(input lepes, char allas[8][8]){
    babu lepobabu;
    bool babu_talalt = false;
    for(int i = 0; i < 6; i++){
        if(lepes.figura == babuk[i]->figura){
            lepobabu = *babuk[i];
            babu_talalt = true;
        }
    }

    if(!babu_talalt) return false; //Na nem sikerült megtalálni a bábut, azaz rossz az input, akkor false return
    if(celmezo_sajat_e(lepes, allas)) return false; //Ha a játékos egy saját bábuját próbálja leütni.

    tavolsagok t;
    t.tavolsagsor = abs(lepes.hovasor - lepes.honnansor);
    t.sortavpozitiv = (lepes.hovasor > lepes.honnansor);
    t.tavolsagoszlop = abs(lepes.hovaoszlop - lepes.honnanoszlop);
    t.oszloptavpozitiv = (lepes.hovaoszlop > lepes.honnanoszlop);
    bool lephet_e;

    switch(lepobabu.figura){
        case 'P':
            lephet_e = gyalog_szabalyos_e(lepes, allas, t);
            break;
        case 'N':
            lephet_e = huszar_szabalyos_e(lepes, allas, t);
            break;
        case 'K':
            lephet_e = kiraly_szabalyos_e(lepes, allas, t);
            break;
        case 'R':
            lephet_e = bastya_szabalyos_e(lepes, allas, t);
            break;
        case 'B':
            lephet_e = futo_szabalyos_e(lepes, allas, t);
            break;
        case 'Q':
            lephet_e = (bastya_szabalyos_e(lepes, allas, t) || futo_szabalyos_e(lepes, allas, t));
            //A királynő szabályosan úgy léphet, mint a futó és a bástya.
            break;
        default:
            lephet_e = false; //Ha valahogyan olyan figura került a változóba, ami nem létezik, akkor egyből false, valamilyen hiba adódott.
            break;
    }
    
    if(lephet_e){
        char temp_allas[8][8];
        allasmegvaltoztatas(lepes, allas);
        return true;
    }

    return false;
}

static bool kiraly_sakkban_e(char allas[8][8], bool vilagos_tamadva_e);

// int main(void){
    // bool sakk = false;
    // bool matt = false;
    // bool passz = false;

    // while (matt != 1) {
    //     char bemenet[5];
    //     for (int sor = 7; sor >= 0; sor--) {
    //         for (int elem = 0; elem < 8; elem++) {
    //             printf("%c ", tabla[sor][elem]);
    //         }
    //         printf("%d ",sor + 1);
    //         printf(" \n");
    //     }

    //     printf("A B C D E F G H \n\n");
    //     printf("Adj meg egy lepest papi! \n");
    //     scanf("%s", bemenet);

    //     input bemenetformazott = mostlepes(bemenet, tabla);
    //     printf("%d, %d, %d, %d, %c\n", bemenetformazott.honnanoszlop, bemenetformazott.honnansor, bemenetformazott.hovaoszlop, bemenetformazott.hovasor, bemenetformazott.figura);

    //     if(szabalyos_lepesforma_e(bemenetformazott, tabla)){
    //         printf("Szabalyos lepes \n");
    //     }else{
    //         printf("Lepj ujra papi mert helytelen\n");
    //     }
    // }

    // return 0;
// }