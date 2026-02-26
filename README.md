# SAKK - Konzolos Sakkprogram C nyelven

Ez a repository egy teljes értékű, karakteres felületen (TUI - Terminal User Interface) futó sakkprogramot tartalmaz, amely C nyelven íródott. A program interaktív játékmenetet kínál, a lépéseket a billentyűzet segítségével lehet megtenni, és támogatja a játékállások elmentését is.

## 📌 Funkciók

- **Teljes sakk logika:** A figurák szabályos mozgásának ellenőrzése, ütés, sakk és sakk-matt detektálása.
- **Interaktív grafikus felület:** Az `econio` könyvtár segítségével a program színes konzolos megjelenítést használ. A játékos kurzorral navigálhat a táblán.
- **Játékállás mentése és betöltése:** A félbehagyott játékok a `Games` mappába menthetők, és később onnan visszatölthetők.
- **Memóriabiztos működés:** A projekt a `debugmalloc` segítségével lett tesztelve a memóriaszivárgások elkerülése érdekében.

## 🏗️ Architektúra és Fájlstruktúra

A forráskód moduláris felépítésű, a könnyebb karbantarthatóság és átláthatóság érdekében az MVC (Model-View-Controller) mintát követi:

- **`main.c`**: A fő játékciklust (game loop) tartalmazó belépési pont. Irányítja a menürendszert és összeköti a program moduljait.
- **`chess.c` / `chess.h`**: A játék magja (Engine / Model). Ezek a fájlok felelnek a sakktábla belső reprezentációjáért és a lépésszabályok validálásáért.
- **`display.c` / `display.h`**: A vizuális megjelenítésért felelős modul (View). Az `econio.h` funkcióira építkezve rajzolja ki a táblát, a figurákat és a menüket.
- **`storage.c` / `storage.h`**: A fájlkezelésért felelős modul. Lehetővé teszi a játékállapot kimentését (szerializáció) és beolvasását a merevlemezről.
- **`econio.c` / `econio.h`**: A konzol terminál manipulálásához szükséges könyvtár (kurzormozgatás, színek, azonnali billentyű-leütés érzékelése).
- **`debugmalloc.h`**: Fejlesztési eszköz a memóriaszivárgások és a hibás mutató-műveletek kiszűrésére.

---

## 🚀 Telepítés és fordítás

A program lefordításához egy C fordítóra (pl. GCC vagy MinGW Windows alatt) van szükség.

### Fordítás parancssorból

Mivel a projekt több forrásfájlból áll, az összes `.c` kiterjesztésű állományt be kell vonni a fordítási folyamatba. Nyiss egy terminált a mappa gyökerében, és futtasd ezt:

```bash
gcc main.c chess.c display.c storage.c econio.c -o sakk.exe
