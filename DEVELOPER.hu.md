# SAKK Fejlesztői Dokumentáció

Ez a dokumentum a SAKK sakkalkalmazás technikai áttekintését nyújtja. Azoknak a fejlesztőknek szól, akik szeretnék megérteni az alkalmazás belső architektúráját, adatszerkezeteit és vezérlési folyamatait.

## Architektúra áttekintés

Az alkalmazás több modulra tagolódik, amelyek mindegyike a játék egy-egy meghatározott aspektusáért felel:

* **`main.c`**: A belépési pont és a központi vezérlő. Ez kezeli az alkalmazás állapotgépét (menük, játékciklus, elemző mód), és koordinálja a többi modult.
* **`chess.c` / `chess.h`**: A sakk alapvető logikáját tartalmazza. Ide tartozik a lépések ellenőrzése, a szabályok érvényesítése (sáncolás, en passant, gyalogátváltozás), valamint a játékállapot felismerése (sakk, sakk-matt, patt).
* **`display.c` / `display.h`**: Kezeli az összes felhasználói felületet és beviteli műveletet. Absztrahálja a konzolos rajzolást és a beviteli olvasást, kommunikálva az `econio` könyvtárral.
* **`storage.c` / `storage.h`**: A játéktörténet adatszerkezetét kezeli. Egy duplán láncolt listát valósít meg (elágazások támogatásával) a táblaállapotok tárolására, lehetővé téve a lépéstörténetben való navigálást, valamint a mentés/betöltés funkciót.
* **`econio.c` / `econio.h`**: Egy egyedi könyvtár a konzolos I/O műveletekhez, amely funkciókat biztosít a színes kimenethez, a kurzor pozicionálásához és a nyers bevitel olvasásához.
* **`debugmalloc.h`**: Egy segédprogram a memóriafoglalások nyomon követésére, amellyel a fejlesztés során kiszűrhetők a memóriaszivárgások.

## Főbb adatszerkezetek

### `Board` (a `storage.h` fájlban)
A `Board` struktúra az alapvető csomópont a játéktörténet láncolt listájában.

```c
typedef struct board {
    char board[8][8];       // A sakktáblát reprezentáló 8x8-as rács
    struct board* prev;     // Mutató az előző lépésre (szülő csomópont)
    int numNext;            // Az ebből a pozícióból induló változatok (gyermek csomópontok) száma
    struct board** next;    // Mutatók tömbje a következő lépésekre (változatokra)
    int id;                 // Egyedi azonosító (a szerializációhoz)
    int previd;             // A szülő azonosítója (a szerializációhoz)
    int selectedBranch;     // A jelenleg kiválasztott változat indexe
    int en_passant_col;     // Oszlopindex az en passant célponthoz (-1, ha nincs)
} Board;
```
> *Megjegyzés: A `next` mutató egy mutatókat tartalmazó tömb (`struct board**`), amely lehetővé teszi, hogy egyetlen pozícióból több elágazás is induljon. Ez kritikus fontosságú az Elemző mód (Analysis Mode) változatainak támogatásához.*

### `Input` (a `chess.h` fájlban)
Egy felhasználói bevitelből vagy a játéklogikából feldolgozott lépést reprezentál.

```c
typedef struct input {
    int orig_row;       // Forrássor (0-7)
    int orig_column;    // Forrásoszlop (0-7)
    int target_row;     // Célsor (0-7)
    int target_column;  // Céloszlop (0-7)
    char figure;        // A mozgatott bábu karaktere
    bool capture;       // Jelző, amely mutatja, ha ütés történt (informatív)
    bool white;         // Igaz, ha a bábu világos, hamis, ha sötét
} Input;
```

### `Booleans` (a `chess.h` fájlban)
Különféle logikai (boolean) jelzőket fog össze a játékállapot nyomon követésére.

```c
typedef struct booleans {
    bool check;             // Sakkban van az aktuális játékos?
    bool mate;              // Sakk-mattot kapott az aktuális játékos?
    bool stalemate;         // Patt helyzetben van a játék?
    bool isWhiteTurn;       // Ki következik? (Igaz, ha világos)
    bool white_king_moved;  // Mozgott már a világos király? (a sáncoláshoz)
    bool black_king_moved;  // Mozgott már a sötét király? (a sáncoláshoz)
    bool white_rook_A_moved;// Mozgott már a világos 'A' bástya? (a sáncoláshoz)
    // ... hasonló jelzők a többi bástyához
    bool playing;           // Aktív jelenleg a játék?
} Booleans;
```

## Alkalmazás folyamata

### Állapotgép (`main.c`)
A `main` függvény egy ciklust futtat, amely a `display.h`-ban definiált `State` enumeráció különböző állapotai között vált:

* `STATE_MAIN_MENU`: Megjeleníti a kezdési lehetőségeket.
* `STATE_GAME_RUNNING`: A fő játékciklus.
* `STATE_ANALYSIS_RUNNING`: Az interaktív visszajátszási és elemző mód.
* ... és egyebek.

Minden állapot egy adott függvénynek felel meg (pl. `game_mode_display`, `analysis_mode_display`), amely az adott mód logikáját kezeli, és visszatér a következő állapottal.

### Lépések ellenőrzése
A lépések ellenőrzését elsődlegesen az `is_move_pattern_valid` függvény végzi a `chess.c` fájlban.

1.  **Szintaktikai ellenőrzés**: A `curr_move` feldolgozza a karakterlánc formátumú bevitelt (pl. "e2e4").
2.  **Logikai ellenőrzés**: Az `is_move_pattern_valid` a következőket vizsgálja:
    * A bábu a saját szabályainak megfelelően mozog?
    * Szabad az út (a csúszó bábuk esetében)?
    * A lépés érvényes ütés vagy csendes lépés?
    * A lépés sakkban hagyja a királyt? (Ezt gyakran a lépés szimulálásával ellenőrzi a program).

### Mentés/Betöltés rendszer
A játékállapot mentése a `Board` csomópontok láncolt listájának szerializálásával történik.

* **Mentés**: A `save_boards` bejárja a lépések fáját, és kiírja minden egyes csomópont adatait (táblaállapot, kapcsolatok) egy fájlba.
* **Betöltés**: A `load_boards` beolvassa a fájlt, újraalkotja a csomópontokat, és az `id`, illetve `previd` alapján újra összekapcsolja őket.

## Fordítás és fejlesztés

A projekt fordításához győződj meg róla, hogy minden forrásfájl jelen van, és futtasd a következő parancsot:

```bash
gcc main.c chess.c display.c storage.c econio.c -o sakk
```

### Hibakeresés (Debugging)
A projekt a `debugmalloc.h`-t használja. A memóriaszivárgás nyomon követésének bekapcsolásához győződj meg arról, hogy be van emelve (include) a `main.c` fájlba. Ez felülírja a szabványos `malloc`/`free` függvényeket a memóriafoglalások követése érdekében.