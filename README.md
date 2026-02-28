# SAKK - Console Chess Program in C

This repository contains a fully featured, character-based (TUI - Terminal User Interface) chess program written in C. The program offers interactive gameplay, allows moves to be made using the keyboard, and supports saving game states.

## 📌 Features

- **Full chess logic:** Validates legal moves for pieces, captures, check, and checkmate detection.
- **Interactive graphical interface:** Uses the `econio` library to provide a colorful console display. Players can navigate the board using a cursor.
- **Save and load games:** Unfinished games can be saved to the `Games` folder and loaded later.
- **Memory-safe operation:** The project was tested using `debugmalloc` to prevent memory leaks.

## 🏗️ Architecture and File Structure

The source code is modular, following the MVC (Model-View-Controller) pattern for easier maintenance and clarity:

- **`main.c`**: The entry point containing the main game loop. It controls the menu system and connects the program's modules.
- **`chess.c` / `chess.h`**: The core of the game (Engine / Model). These files handle the internal representation of the chessboard and validate move rules.
- **`display.c` / `display.h`**: The module responsible for visual presentation (View). Built on `econio.h` functions, it draws the board, pieces, and menus.
- **`storage.c` / `storage.h`**: The file management module. It allows saving (serialization) and loading game states from the hard drive.
- **`econio.c` / `econio.h`**: A library necessary for console terminal manipulation (cursor movement, colors, detecting immediate keystrokes).
- **`debugmalloc.h`**: A development tool for filtering out memory leaks and invalid pointer operations.

---

## 🚀 Installation and Compilation

To compile the program, you need a C compiler (e.g., GCC or MinGW on Windows).

### Compiling from the command line

Since the project consists of multiple source files, all `.c` extension files must be included in the compilation process. Open a terminal in the root folder and run this:

```bash
gcc main.c chess.c display.c storage.c econio.c -o sakk.exe
