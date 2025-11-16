#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "debugmalloc.h"
#include "Adat.h"
#include "Sakk.h"

Board* head = NULL;
char tabla[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
};

int main(){
    bool sakk = false;
    bool matt = false;
    bool passz = false;

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

        if(szabalyos_lepesforma_e(bemenetformazott, tabla)){
            printf("Szabalyos lepes \n");
        }else{
            printf("Lepj ujra papi mert helytelen\n");
        }
    }

    return 0;   
}
