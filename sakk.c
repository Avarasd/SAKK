#include <stdio.h>
#include <stdbool.h>

struct boolok
{
    bool sakk;
    bool matt;
    bool passz;
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
    char figura;
    char hova[4];
    bool utes;
} input;
babu gyalogos = {'P', 1, 0, 0, 0, 1};
babu bastya = {'R', 7, 7, 0, 1};
babu huszar = {'N', 2, 2, 0, 1};
babu futo = {'B', 0, 0, 7, 1};
babu kiralyno = {'Q', 7, 7, 7, 1};
babu kiraly = {'K', 1, 1, 1, 1, 1};

char *babuk[6] = {&gyalogos, &bastya, &huszar, &futo, &kiralyno, &kiraly};
input mostlepes(char lepes[4])
{
    input ki;
    bool gyalog = true;
    for (int i = 0; i < 6; i++)
    {
        if (babuk[i].figura == lepes[0])
        {
            ki.figura = lepes[0];
            gyalog = false;
        }
        if (gyalog)
        {
            ki.figura = 'P';
        }
    }
    input asd;
    return asd;
};
int main(void)
{
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
    while (matt != 1)
    {

        for (int sor = 7; sor >= 0; sor--)
        {
            for (int elem = 0; elem < 8; elem++)
            {
                printf("%c ", tabla[sor][elem]);
            }
            printf("\n");
        }
        // definialni a babukat
        //
    }

    return 0;
}