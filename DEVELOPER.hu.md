# SAKK Fejlesztői Dokumentáció

Ez a dokumentum a SAKK sakkalkalmazás technikai áttekintését nyújtja. Azoknak a fejlesztőknek szól, akik szeretnék megérteni az alkalmazás belső architektúráját, adatszerkezeteit és vezérlési folyamatait.

## Architektúra áttekintés

Az alkalmazás több modulra tagolódik, amelyek mindegyike a játék egy-egy meghatározott aspektusáért felel:

- **`main.c`**: A belépési pont és a központi vezérlő. Ez kezeli az alkalmazás állapotgépét (menük, játékciklus, elemző mód), és koordinálja a többi modult.
- **`chess.c` / `chess.h`**: A sakk alapvető logikáját tartalmazza. Ide tartozik a lépések ellenőrzése, a szabályok érvényesítése (sáncolás, en passant, gyalogátváltozás), valamint a játékállapot felismerése (sakk, sakk-matt, patt).
- **`display.c` / `display.h`**: Kezeli az összes felhasználói felületet és beviteli műveletet. Absztrahálja a konzolos rajzolást és a beviteli olvasást, kommunikálva az `econio` könyvtárral.
- **`storage.c` / `storage.h`**: A játéktörténet adatszerkezetét kezeli. Egy faestruktúrát valósít meg (elágazások támogatásával) a táblaállapotok tárolására, lehetővé téve a lépéstörténetben való navigálást, valamint a mentés/betöltés funkciót.
- **`econio.c` / `econio.h`**: Egy egyedi könyvtár a konzolos I/O műveletekhez, amely funkciókat biztosít a színes kimenethez, a kurzor pozicionálásához és a nyers bevitel olvasásához.
- **`debugmalloc.h`**: Egy segédprogram a memóriafoglalások nyomon követésére, amellyel a fejlesztés során kiszűrhetők a memóriaszivárgások.

---

## Bábuk kódolási konvenciója

> **Ez a legfontosabb dolog, amit meg kell érteni, mielőtt bármilyen sakklogikát olvasol.**

A tábla egy `char[8][8]` tömbként van reprezentálva. Minden cella egy karaktert tartalmaz, amely az adott mezőn lévő bábut jelöli, vagy `'.'`-t, ha a mező üres.

**A világos bábuk kisbetűk, a sötét bábuk nagybetűk.** Ez az általános sakkkonvencióval *ellentétes*, ezért figyelj oda alaposan.

| Bábu   | Világos (kisbetű) | Sötét (nagybetű) |
|--------|:-----------------:|:----------------:|
| Király | `k`               | `K`              |
| Vezér  | `q`               | `Q`              |
| Bástya | `r`               | `R`              |
| Futó   | `b`               | `B`              |
| Huszár | `n`               | `N`              |
| Gyalog | `p`               | `P`              |
| Üres   | `.`               | `.`              |

A `SMALL_CAP_DISTANCE 32` konstans (a `chess.h`-ban definiálva) egy kisbetű és a megfelelő nagybetű közötti ASCII-különbség (`'a' - 'A' == 32`). A motor egész kódjában ezt az értéket használják a világos és sötét bábuk representációja közötti átváltáshoz.

Annak ellenőrzése, hogy egy bábu világos-e: `piece > 'A' + SMALL_CAP_DISTANCE` (azaz a karakter kisbetű).

---

## A lépésbevitel formátuma

A lépéseket egy 4 karakteres karakterlánc formájában kell megadni: **`OSZOS`**
- **O** — Oszlopbetű (`A`–`H`), nagybetű
- **S** — Sorszám (`1`–`8`)

Tehát az E2-ről E4-re lépést a következőképpen kell megadni: `E2E4`

A `chess.c`-ben lévő `curr_move()` függvény ezt a karakterláncot egy `Input` struktúrává alakítja, a bábu típusát és színét a táblaállapotból feloldva.

---

## Főbb adatszerkezetek

### `Board` (a `storage.h` fájlban)
A `Board` struktúra az alapvető csomópont a játéktörténet fájában.

```c
typedef struct board {
    char board[8][8];       // A sakktáblát reprezentáló 8x8-as rács
    struct board* prev;     // Mutató az előző lépésre (szülő csomópont)
    int numNext;            // Az ebből a pozícióból induló változatok (gyermek csomópontok) száma
    struct board** next;    // Dinamikusan allokált mutatótömb a gyermek csomópontokra
    int id;                 // Egyedi azonosító (a szerializációhoz)
    int previd;             // A szülő azonosítója (a szerializációhoz)
    int selectedBranch;     // A jelenleg kiválasztott változat indexe
    int en_passant_col;     // Oszlopindex (0–7) az en passant célponthoz; -1, ha nincs
} Board;
```

A `next` mutató egy **dinamikusan allokált tömb** (`struct board**`), amelyet az `add_new_board()` függvény bővít `realloc`-kal. Ez lehetővé teszi, hogy egyetlen pozícióból több elágazás is induljon, ami az Elemző mód változattámogatásának alapja.

### `Input` (a `chess.h` fájlban)
Egy felhasználói bevitelből vagy a játéklogikából feldolgozott lépést reprezentál.

```c
typedef struct input {
    int orig_row;       // Forrássor (0–7)
    int orig_column;    // Forrásoszlop (0–7)
    int target_row;     // Célsor (0–7)
    int target_column;  // Céloszlop (0–7)
    char figure;        // A bábu nagybetűs karaktere (pl. 'K', 'P')
    bool capture;       // Igaz, ha a célmező egy ellenséges bábut tartalmaz
    bool white;         // Igaz, ha a mozgó bábu világos (kisbetű a táblán)
} Input;
```

*Megjegyzés: A `figure` mindig nagybetűsen van tárolva, a színtől függetlenül. A `white` mező tárolja a szín információját.*

### `Booleans` (a `chess.h` fájlban)
Különféle logikai (boolean) jelzőket fog össze a játékállapot nyomon követésére.

```c
typedef struct booleans {
    bool check;               // Sakkban van az aktuális játékos?
    bool mate;                // Sakk-mattot kapott az aktuális játékos?
    bool stalemate;           // Patt helyzetben van a játék?
    bool isWhiteTurn;         // Igaz, ha a világos következik
    bool white_king_moved;    // Mozgott már a világos király? (tiltja a sáncolást)
    bool black_king_moved;    // Mozgott már a sötét király? (tiltja a sáncolást)
    bool white_rook_A_moved;  // Mozgott már a világos A-oldali bástya? (tiltja a vezérszárnyit)
    bool white_rook_H_moved;  // Mozgott már a világos H-oldali bástya? (tiltja a királyiszárnyit)
    bool black_rook_A_moved;  // Mozgott már a sötét A-oldali bástya? (tiltja a vezérszárnyit)
    bool black_rook_H_moved;  // Mozgott már a sötét H-oldali bástya? (tiltja a királyiszárnyit)
    bool playing;             // Aktív jelenleg a játék?
} Booleans;
```

### `Distances` (a `chess.h` fájlban)
Egy segédstruktúra, amelyet egy `Input` lépésből számítanak ki, és a bábumozgás-ellenőrzők geometriai számításait egyszerűsíti.

```c
typedef struct distances {
    int row_distance;         // Abszolút sortávolság a forrás és a cél között
    bool row_distance_pos;    // Igaz, ha a lépés pozitív sorizányba mutat (felfelé)
    int column_distance;      // Abszolút oszloptávolság a forrás és a cél között
    bool column_distance_pos; // Igaz, ha a lépés pozitív oszlopirányba mutat (jobbra)
} Distances;
```

A `setup_distances()` tölti fel ezt a struktúrát egy `Input` alapján, majd átadja a bábu-specifikus ellenőrzőknek, mint az `is_rook_valid()`, `is_bishop_valid()` stb.

---

## Alkalmazás folyamata

### Állapotgép (`main.c`)
A `main` függvény egy ciklust futtat, amely a `display.h`-ban definiált `State` enumeráció állapotai között vált. Minden állapot egy kezelő függvénynek felel meg, amely elvégzi a feladatát, és visszatér a **következő állapottal**.

| Állapot                  | Kezelő függvény            | Leírás                                                     |
|--------------------------|----------------------------|------------------------------------------------------------|
| `STATE_MAIN_MENU`        | `display_menu()`           | Megjeleníti a főmenüt                                      |
| `STATE_GAME_MENU`        | `game_mode_menu()`         | Megerősítő képernyő a játék indítása előtt                 |
| `STATE_GAME_RUNNING`     | `game_mode_display()`      | A fő kétjátékos játékciklus                                |
| `STATE_GAME_END`         | `game_mode_end()`          | Mentési fájlnevet kér a játék vége után                    |
| `STATE_ANALYSIS_MENU`    | `analysis_mode_menu()`     | Megerősítő képernyő az elemző mód előtt                    |
| `STATE_ANALYSIS_SETUP`   | `analysis_mode_file_set()` | Fájlválasztó mentett játék betöltéséhez                    |
| `STATE_ANALYSIS_RUNNING` | `analysis_mode_display()`  | Interaktív lépésfa visszajátszás és elemzés                |
| `STATE_ANALYSIS_END`     | *(main-ben inline)*        | Elmenti az elemzési változásokat és visszatér a főmenübe   |
| `STATE_EXIT`             | *(main-ben inline)*        | Kilép az alkalmazás ciklusból                              |

### Lépésellenőrzési folyamat
A lépések ellenőrzését az `is_move_pattern_valid()` függvény végzi a `chess.c`-ben, amely a következő ellenőrzéseket hajtja végre sorban:

1. **Szín ellenőrzés**: A mozgó bábu színének egyeznie kell az `isWhiteTurn` értékével.
2. **Geometriai ellenőrzés** (`check_geometry_only`):
   - Ismert-e a `figure` bábufajta?
   - Saját bábut üt-e? (célmező azonos színű bábut tartalmaz)
   - A bábu a saját mozgásszabályai szerint mozog-e? (bábu-specifikus ellenőrző meghívása)
3. **Sáncolás tartalék**: Ha a geometriai ellenőrzés sikertelen és a bábu Király, az `is_castling_valid()` kerül meghívásra.
4. **Önsakk szimuláció**: A lépés egy ideiglenes táblakópiára kerül alkalmazásra. Ha az eredmény a lépő játékos Királyát sakkban hagyja, a lépés elutasításra kerül.
5. **Táblamódosítás** (csak ha `modifyBoard == true`): A tényleges tábla frissül, a sáncolásnál a bástya átkerül, és a következő forduló en passant céloszlopa beállításra kerül.

### Mentés/Betöltés rendszer
A játékállapot mentése a `Board` csomópontok fájának bináris fájlba való szerializálásával történik egy lapos `BoardElement` struktúra segítségével. Az `id`/`previd` mezők idegen kulcsként működnek, amelyek segítségével betöltéskor a szülő-gyermek kapcsolatok rekonstruálhatók.

- **Mentés** (`save_boards` → `saveboard`): Mélységi bejárással végigmegy a fán, és minden csomópontot `BoardElement`-ként ír ki `fwrite`-tal.
- **Betöltés** (`load_boards` → `create_board_from_element`): `fread`-del beolvassa az egyes `BoardElement`-eket. Ha `previd == -1`, az a gyökércsomópont. Egyébként a `search_board()` megkeresi a szülőt, és az `add_new_board()` csatolja a gyermeket.

---

## Fordítás és fejlesztés

A projekt fordításához győződj meg róla, hogy minden forrásfájl jelen van, és futtasd a következő parancsot:

```bash
gcc main.c chess.c display.c storage.c econio.c -o sakk -Wall -Wextra -std=c99
```

| Kapcsoló  | Célja                                                                |
|-----------|----------------------------------------------------------------------|
| `-Wall`   | Az összes standard fordítói figyelmeztetés engedélyezése            |
| `-Wextra` | A `-Wall`-on túli további figyelmeztetések engedélyezése            |
| `-std=c99`| C99 szabványt céloz (szükséges a `stdbool.h`-hoz)                   |

### Memória hibakeresés
A projekt a `debugmalloc.h`-t használja. Ez felülírja a szabványos `malloc`/`free`/`realloc` függvényeket az összes memóriafoglalás nyomon követéséhez. Ha a program kilépésekor bármely memória nincs felszabadítva, hibajelentést ír a stderr-re. A használathoz egyszerűen győződj meg arról, hogy a `#include "debugmalloc.h"` jelen van a `main.c`-ben (csak **egy** fordítási egységbe szabad belefoglalni).