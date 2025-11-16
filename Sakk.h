extern char tabla[8][8];
#define SMALL_CAP_DISTANCE 32


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

typedef struct tavolsagok{
    int tavolsagsor;
    bool sortavpozitiv;
    int tavolsagoszlop;
    bool oszloptavpozitiv;
} tavolsagok;

input mostlepes(char lepes[5], char allas[8][8]);

bool szabalyos_lepesforma_e(input lepes, char allas[8][8]);

