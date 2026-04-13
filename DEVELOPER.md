# SAKK Developer Documentation

This document provides a technical overview of the SAKK chess application. It is intended for developers who wish to understand the internal architecture, data structures, and control flow of the application.

## Architecture Overview

The application is structured into several modules, each responsible for a specific aspect of the game:

- **`main.c`**: The entry point and central controller. It manages the application state machine (menus, game loop, analysis mode) and coordinates between other modules.
- **`chess.c` / `chess.h`**: Contains the core chess logic. This includes move validation, rule enforcement (castling, en passant, promotion), and game state detection (check, checkmate, stalemate).
- **`display.c` / `display.h`**: Handles all user interface and input operations. It abstracts the console drawing and input reading, communicating with the `econio` library.
- **`storage.c` / `storage.h`**: Manages the game history data structure. It implements a tree structure (with branching support) of board states, enabling move history navigation and save/load functionality.
- **`econio.c` / `econio.h`**: A custom library for console I/O, providing functions for colored output, cursor positioning, and raw input reading.
- **`debugmalloc.h`**: A utility for tracking memory allocations to detect leaks during development.

---

## Piece Encoding Convention

> **This is the most important thing to understand before reading any of the chess logic.**

The board is represented as a `char[8][8]` array. Each cell contains a character representing the piece on that square, or `'.'` for an empty square.

**White pieces use lowercase letters, black pieces use uppercase letters.** This is the *opposite* of the standard chess convention, so pay close attention.

| Piece  | White (lowercase) | Black (uppercase) |
|--------|:-----------------:|:-----------------:|
| King   | `k`               | `K`               |
| Queen  | `q`               | `Q`               |
| Rook   | `r`               | `R`               |
| Bishop | `b`               | `B`               |
| Knight | `n`               | `N`               |
| Pawn   | `p`               | `P`               |
| Empty  | `.`               | `.`               |

The constant `SMALL_CAP_DISTANCE 32` (defined in `chess.h`) is the ASCII distance between a lowercase and its corresponding uppercase letter (e.g., `'a' - 'A' == 32`). It is used throughout the engine to convert between white and black piece representations.

To check if a piece is white: `piece > 'A' + SMALL_CAP_DISTANCE` (i.e., the character is lowercase).

---

## Move Input Format

Moves are entered as a 4-character string in the format: **`CRCT`**
- **C** — Column letter (`A`–`H`), uppercase
- **R** — Row number (`1`–`8`)

So a move from E2 to E4 is entered as: `E2E4`

The `curr_move()` function in `chess.c` parses this string into an `Input` struct, resolving the piece type and color from the board state.

---

## Key Data Structures

### `Board` (in `storage.h`)
The `Board` struct is the fundamental node in the game history tree.

```c
typedef struct board {
    char board[8][8];       // The 8x8 grid representing the chess board
    struct board* prev;     // Pointer to the previous move (parent node)
    int numNext;            // Number of variations (children nodes) from this position
    struct board** next;    // Dynamically allocated array of pointers to child nodes
    int id;                 // Unique identifier (for serialization)
    int previd;             // Parent's identifier (for serialization)
    int selectedBranch;     // Index of the currently selected variation
    int en_passant_col;     // Column index (0–7) of en passant target; -1 if none
} Board;
```

The `next` pointer is a **dynamically allocated array** (`struct board**`), grown with `realloc` in `add_new_board()`. This allows multiple branches from a single position, which is the foundation of the Analysis Mode's variation support.

### `Input` (in `chess.h`)
Represents a move parsed from user input or game logic.

```c
typedef struct input {
    int orig_row;       // Source row (0–7)
    int orig_column;    // Source column (0–7)
    int target_row;     // Destination row (0–7)
    int target_column;  // Destination column (0–7)
    char figure;        // The uppercase character of the piece (e.g., 'K', 'P')
    bool capture;       // True if the target square is occupied by an enemy piece
    bool white;         // True if the moving piece is white (lowercase on board)
} Input;
```

*Note: `figure` is always stored as uppercase regardless of color. The `white` field carries the color information.*

### `Booleans` (in `chess.h`)
Aggregates various boolean flags to track the game state.

```c
typedef struct booleans {
    bool check;               // Is the current player in check?
    bool mate;                // Is the current player checkmated?
    bool stalemate;           // Is the game in stalemate?
    bool isWhiteTurn;         // True if it is White's turn
    bool white_king_moved;    // Has the White King moved? (disables White castling)
    bool black_king_moved;    // Has the Black King moved? (disables Black castling)
    bool white_rook_A_moved;  // Has the White A-side Rook moved? (disables queenside castle)
    bool white_rook_H_moved;  // Has the White H-side Rook moved? (disables kingside castle)
    bool black_rook_A_moved;  // Has the Black A-side Rook moved? (disables queenside castle)
    bool black_rook_H_moved;  // Has the Black H-side Rook moved? (disables kingside castle)
    bool playing;             // Is the game currently active?
} Booleans;
```

### `Distances` (in `chess.h`)
A helper struct computed from an `Input` move, used to simplify geometry checks in piece movement validators.

```c
typedef struct distances {
    int row_distance;         // Absolute row distance between source and target
    bool row_distance_pos;    // True if the move goes in the positive row direction (upward)
    int column_distance;      // Absolute column distance between source and target
    bool column_distance_pos; // True if the move goes in the positive column direction (rightward)
} Distances;
```

`setup_distances()` populates this struct from an `Input`. It is then passed to per-piece validators like `is_rook_valid()`, `is_bishop_valid()`, etc.

---

## Application Flow

### State Machine (`main.c`)
The `main` function runs a loop that switches between states defined in the `State` enum (in `display.h`). Each state corresponds to a handler function that performs its work and returns the **next state**.

| State                  | Handler Function          | Description                                      |
|------------------------|---------------------------|--------------------------------------------------|
| `STATE_MAIN_MENU`      | `display_menu()`          | Shows the main menu                              |
| `STATE_GAME_MENU`      | `game_mode_menu()`        | Confirmation screen before starting a game       |
| `STATE_GAME_RUNNING`   | `game_mode_display()`     | The main two-player game loop                    |
| `STATE_GAME_END`       | `game_mode_end()`         | Prompts for a save filename after the game ends  |
| `STATE_ANALYSIS_MENU`  | `analysis_mode_menu()`    | Confirmation screen before entering analysis     |
| `STATE_ANALYSIS_SETUP` | `analysis_mode_file_set()`| File picker for loading a saved game             |
| `STATE_ANALYSIS_RUNNING`| `analysis_mode_display()` | Interactive move-tree replay and annotation      |
| `STATE_ANALYSIS_END`   | *(inline in main)*        | Saves analysis changes and returns to main menu  |
| `STATE_EXIT`           | *(inline in main)*        | Exits the application loop                       |

### Move Validation Pipeline
Move validation is handled by `is_move_pattern_valid()` in `chess.c`, which runs the following checks in order:

1. **Color check**: The moving piece's color must match `isWhiteTurn`.
2. **Geometry check** (`check_geometry_only`):
   - Is `figure` a recognized piece type?
   - Is it a friendly-fire move? (targeting a square occupied by the same color)
   - Does the piece move according to its own movement rules? (calls the per-piece validator)
3. **Castling fallback**: If geometry fails and the piece is a King, `is_castling_valid()` is tried.
4. **Self-check simulation**: The move is applied to a temporary board copy. If the result leaves the mover's King in check, the move is rejected.
5. **Board mutation** (only if `modifyBoard == true`): The actual board is updated, rook repositioning for castling is applied, and the en passant target column for the next turn is set.

### Save/Load System
The game state is saved by serializing the tree of `Board` nodes to a binary file using a flat `BoardElement` struct. The `id`/`previd` fields act as foreign keys to reconstruct the parent-child relationships on load.

- **Saving** (`save_boards` → `saveboard`): Traverses the tree depth-first and writes each node as a `BoardElement` via `fwrite`.
- **Loading** (`load_boards` → `create_board_from_element`): Reads each `BoardElement` with `fread`. If `previd == -1`, it's the root node. Otherwise, it uses `search_board()` to find the parent and calls `add_new_board()` to attach the child.

---

## Building and Development

To compile the project, ensure all source files are present and run:

```bash
gcc main.c chess.c display.c storage.c econio.c -o sakk -Wall -Wextra -std=c99
```

| Flag      | Purpose                                              |
|-----------|------------------------------------------------------|
| `-Wall`   | Enables all standard compiler warnings               |
| `-Wextra` | Enables additional warnings beyond `-Wall`           |
| `-std=c99`| Targets the C99 standard (required for `stdbool.h`)  |

### Debugging Memory
The project uses `debugmalloc.h`. It overrides standard `malloc`/`free`/`realloc` to track all allocations. If any memory is not freed by program exit, it prints a report to stderr. To use it, simply ensure `#include "debugmalloc.h"` is present in `main.c` (it must be included in only **one** translation unit).
