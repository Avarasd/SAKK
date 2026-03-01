# Hungarian Console Chess Program in C

This repository contains a fully featured, character-based (TUI - Terminal User Interface) chess program written in C. The program offers interactive two-player gameplay, supports a game analysis mode with branching move trees, and saves game states to disk.

## 📌 Features

- **Full chess logic:** Validates legal moves for all pieces, including captures, check, checkmate, stalemate, castling, en passant, and pawn promotion.
- **Colorful terminal interface:** Uses the `econio` library to provide a colorful console display with board visualization.
- **Game analysis mode:** Load a saved game and replay it move by move. Supports branching variations — you can explore alternative lines from any position.
- **Save and load games:** Finished or in-progress games can be saved to the `Games` folder and loaded later.
- **Memory-safe operation:** The project was tested using `debugmalloc` to prevent memory leaks.

## 🎮 How to Play

Moves are entered as a 4-character string representing the source and destination squares:

```
<column><row><column><row>   →   e.g. E2E4 (move piece from E2 to E4)
```

- Columns are letters `A`–`H`, rows are numbers `1`–`8`, both uppercase.
- The program will notify you if a move is illegal.

### Analysis Mode Controls

| Key       | Action                              |
|-----------|-------------------------------------|
| `←` / `→` | Step backward / forward in the game |
| `1`       | Jump to starting position           |
| `2`       | Jump to the last move               |
| `3`       | Jump to a specific move number      |
| `4`       | Create a new variation from here    |
| `5`       | Switch to a different branch        |
| `X`       | Exit analysis mode                  |

> **Note:** The game file browser in analysis mode is currently Windows-only.

## 🏗️ Architecture and File Structure

The source code is modular, following the MVC (Model-View-Controller) pattern for easier maintenance and clarity:

- **`main.c`**: The entry point containing the main game loop. It controls the menu system and connects the program's modules.
- **`chess.c` / `chess.h`**: The core of the game (Engine / Model). These files handle the internal representation of the chessboard and validate move rules.
- **`display.c` / `display.h`**: The module responsible for visual presentation (View). Built on `econio.h` functions, it draws the board, pieces, and menus.
- **`storage.c` / `storage.h`**: The file management module. It allows saving (serialization) and loading game states from disk, and manages the branching move-history tree.
- **`econio.c` / `econio.h`**: A library for console terminal manipulation (cursor movement, colors, detecting immediate keystrokes).
- **`debugmalloc.h`**: A development tool for detecting memory leaks and invalid pointer operations.

---

## 🚀 Installation and Compilation

To compile the program, you need a C compiler (e.g., GCC or MinGW on Windows).

### Compiling from the command line

Since the project consists of multiple source files, all `.c` files must be included in the compilation. Open a terminal in the root folder and run:

```bash
gcc main.c chess.c display.c storage.c econio.c -o chess-tui -Wall -Wextra -std=c99
```

For more details on the internal architecture, see [DEVELOPER.md](DEVELOPER.md) (also available in [Hungarian](DEVELOPER.hu.md)).
