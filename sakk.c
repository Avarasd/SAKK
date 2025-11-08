#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#define SMALL_CAP_DISTANCE 32

struct boolok
{
    bool sakk;
    bool matt;
    bool patt;
};

typedef struct babu
{
    char figura;
    int lepeselore;
    int lepesoldalra;
    int lepesatlosan;
    bool hatralepes;
    bool elsolepes;
} babu;
typedef struct input
{
    int honnansor;
    int honnanoszlop;
    int hovasor;
    int hovaoszlop;
    char figura;
    bool utes;
    bool vilagos;
} input;

typedef struct board{
    char allas;
    Board* prev;
    int numNext;
    Board** next;
}Board;

static Board* head = NULL;

char tabla[8][8];

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
    printf("%c \n", tabla[ki.honnansor][ki.honnanoszlop]);
    printf("ELSO CHECKPOINT %c \n", ki.figura);
    ki.utes = !(tabla[ki.hovasor][ki.hovaoszlop] = '.');
    printf("%d\n", ki.utes);
    return ki;
};
void allasmegvaltoztatas(input lepes, char allas[8][8]){
    if(lepes.vilagos){
        allas[lepes.hovasor][lepes.hovaoszlop] = lepes.figura + SMALL_CAP_DISTANCE;
    }else allas[lepes.hovasor][lepes.hovaoszlop] = lepes.figura;

    allas[lepes.honnansor][lepes.honnanoszlop] = '.';
}
bool szabalyoslepes(input lepes, char allas[8][8]){
    babu lepobabu;
    for(int i = 0; i < 6; i++){
        if(lepes.figura == babuk[i]->figura){
            lepobabu = *babuk[i];
        }
    }

    int tavolsagsor = abs(lepes.hovasor - lepes.honnansor);
    bool sortavpozitiv = (lepes.hovasor > lepes.honnansor);
    int tavolsagoszlop = abs(lepes.hovaoszlop - lepes.honnanoszlop);
    bool oszloptavpozitiv = (lepes.hovaoszlop > lepes.honnanoszlop);

    switch(lepobabu.figura){
        case 'N':
            if((tavolsagsor == 1 && tavolsagoszlop == 2) || (tavolsagsor == 2 && tavolsagoszlop == 1)){
                allasmegvaltoztatas(lepes, allas);
            }
        break;
        case 'P':
            if(lepes.vilagos){
                if(lepes.utes){
                    if(tavolsagsor == tavolsagoszlop == 1 && sortavpozitiv){
                        allasmegvaltoztatas(lepes, allas);
                }
                }else{
                    if((tavolsagsor == 1 && sortavpozitiv) || (tavolsagsor == 2 && sortavpozitiv && lepes.honnansor == 1)){
                        for(int i = 1; i <=tavolsagsor; i++){
                            if(allas[lepes.honnansor + i][lepes.honnanoszlop] != '.'){
                                return false;
                            }
                        }
                        allasmegvaltoztatas(lepes, allas);
                    }
                }  
            }else{
                if(lepes.utes){
                    if(tavolsagsor == tavolsagoszlop == 1 && !sortavpozitiv){
                        allasmegvaltoztatas(lepes, allas);
                    }
                }else{
                    if((tavolsagsor == 1 && !sortavpozitiv) || (tavolsagsor == 2 && !sortavpozitiv && lepes.honnansor == 6)){
                        for(int i = 1; i <= tavolsagsor; i++){
                            if(allas[lepes.honnansor - i][lepes.honnanoszlop] != '.'){
                                return false;
                            }
                        }
                        allasmegvaltoztatas(lepes,allas);
                    }
                }
            }  
        break;
        default:
            if(tavolsagsor <= lepobabu.lepeselore && tavolsagoszlop == 0){
                if(sortavpozitiv){
                    for(int i = 1; i < tavolsagsor; i++){
                        if(allas[lepes.honnansor + i][lepes.honnanoszlop] != '.'){
                            return false;
                        }
                    }
                }else{
                    for(int i = 1; i< tavolsagsor; i++){
                        if(allas[lepes.honnansor - i][lepes.honnanoszlop] != '.'){
                            return false;
                        }
                    }
                }
                allasmegvaltoztatas(lepes, allas);
            }
            if(tavolsagoszlop <= lepobabu.lepesoldalra && tavolsagsor == 0){
                if(oszloptavpozitiv){
                    for(int i = 1; i < tavolsagoszlop; i++){
                        if(allas[lepes.honnansor][lepes.honnanoszlop + i] != '.'){
                            return false;
                        }
                    }
                }else{
                    for(int i = 1; i < tavolsagoszlop; i++){
                        if(allas[lepes.honnansor][lepes.honnanoszlop - i] != '.'){
                            return false;
                        }
                    }
                }
                allasmegvaltoztatas(lepes, allas);
            }
            if(tavolsagsor == tavolsagoszlop && tavolsagsor <= lepobabu.lepesatlosan){
                if(sortavpozitiv && oszloptavpozitiv){
                    for(int i = 1; i < tavolsagoszlop; i++){
                        if(allas[lepes.honnansor + i][lepes.honnanoszlop + i] != '.'){
                            return false;
                        }
                    }
                }
                else if(sortavpozitiv && !oszloptavpozitiv){
                    for(int i = 1; i <tavolsagoszlop; i++){
                        if(allas[lepes.honnansor + i][lepes.honnanoszlop - i] != '.'){
                            return false;
                        }
                    }
                }
                else if(!sortavpozitiv && oszloptavpozitiv){
                    for(int i = 1; i < tavolsagoszlop; i++){
                        if(allas[lepes.honnansor - i][lepes.honnanoszlop + i] != '.'){
                            return false;
                        }
                    }
                }
                else if(!sortavpozitiv && !oszloptavpozitiv){
                    for(int i = 1; i < tavolsagoszlop; i++){
                        if(allas[lepes.honnansor - i][lepes.honnanoszlop - i] != '.'){
                            return false;
                        }
                    }
                }
                allasmegvaltoztatas(lepes, allas);
            }
    }
}
bool sakkhelyzet(input lepes){
    for(int sor = 0; sor < 8; sor++){
        for(int mezo = 0; mezo < 8; mezo++){
            if(tabla[sor][mezo] == 'K'){
                input kovetkezolepes = {lepes.honnansor, lepes.honnanoszlop, sor, mezo, lepes.figura};
                if(szabalyoslepes(kovetkezolepes, tabla)){
                    return true;
                }
            }
        }
    }
    return false;
}
int main(void) {
    bool sakk = false;
    bool matt = false;
    bool passz = false;
    char tabla[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}};

    while (matt != 1) {
        char bemenet[5];
        for (int sor = 7; sor >= 0; sor--) {
            for (int elem = 0; elem < 8; elem++) {
                printf("%c ", tabla[sor][elem]);
            }
            printf("%d ",sor + 1);
            printf(" \n");
        }

        printf("A B C D E F G H \n\n");
        printf("Adj meg egy lepest papi! \n");
        scanf("%s", bemenet);

        input bemenetformazott = mostlepes(bemenet, tabla);
        printf("%d, %d, %d, %d, %c\n", bemenetformazott.honnanoszlop, bemenetformazott.honnansor, bemenetformazott.hovaoszlop, bemenetformazott.hovasor, bemenetformazott.figura);

        if(szabalyoslepes(bemenetformazott, tabla) == 0){
            printf("Szabalyos lepes \n");
        }else{
            printf("Lepj ujra papi mert helytelen\n");
        }
    }

    return 0;
}