# SAKK Developer Documentation

This document provides a technical overview of the SAKK chess application. It is intended for developers who wish to understand the internal architecture, data structures, and control flow of the application.

## Architecture Overview

The application is structured into several modules, each responsible for a specific aspect of the game:

- **`main.c`**: The entry point and central controller. It manages the application state machine (menus, game loop, analysis mode) and coordinates between other modules.
- **`chess.c` / `chess.h`**: Contains the core chess logic. This includes move validation, rule enforcement (castling, en passant, promotion), and game state detection (check, checkmate, stalemate).
- **`display.c` / `display.h`**: Handles all user interface and input operations. It abstracts the console drawing and input reading, communicating with the `econio` library.
- **`storage.c` / `storage.h`**: Manages the game history data structure. It implements a doubly linked list (with branching support) to store board states, enabling move history navigation and save/load functionality.
- **`econio.c` / `econio.h`**: A custom library for console I/O, providing functions for colored output, cursor positioning, and raw input reading.
- **`debugmalloc.h`**: A utility for tracking memory allocations to detect leaks during development.

## Key Data Structures

### `Board` (in `storage.h`)
The `Board` struct is the fundamental node in the game history linked list.

```c
typedef struct board {
    char board[8][8];       // The 8x8 grid representing the chess board
    struct board* prev;     // Pointer to the previous move (parent node)
    int numNext;            // Number of variations (children nodes) from this position
    struct board** next;    // Array of pointers to next moves (variations)
    int id;                 // Unique identifier (for serialization)
    int previd;             // Parent's identifier (for serialization)
    int selectedBranch;     // Index of the currently selected variation
    int en_passant_col;     // Column index for en passant target (-1 if none)
} Board;
```
*Note: The `next` pointer is an array of pointers (`struct board**`), allowing for multiple branches from a single position. This is crucial for the Analysis Mode's variation support.*

### `Input` (in `chess.h`)
Represents a move parsed from user input or game logic.

```c
typedef struct input {
    int orig_row;       // Source row (0-7)
    int orig_column;    // Source column (0-7)
    int target_row;     // Destination row (0-7)
    int target_column;  // Destination column (0-7)
    char figure;        // The character of the piece being moved
    bool capture;       // Flag indicating if a capture occurred (informational)
    bool white;         // True if the piece is white, false if black 
} Input;
```

### `Booleans` (in `chess.h`)
Aggregates various boolean flags to track the game state.

```c
typedef struct booleans {
    bool check;             // Is the current player in check?
    bool mate;              // Is the current player checkmated?
    bool stalemate;         // Is the game in a stalemate?
    bool isWhiteTurn;       // Whose turn is it?
    bool white_king_moved;  // Has White King moved? (for castling)
    bool black_king_moved;  // Has Black King moved? (for castling)
    bool white_rook_A_moved;// Has White Rook A moved? (for castling)
    // ... similar flags for other rooks
    bool playing;           // Is the game currently active?
} Booleans;
```

## Application Flow

### State Machine (`main.c`)
The `main` function runs a loop that switches between different states defined in the `State` enum (in `display.h`):

- `STATE_MAIN_MENU`: Shows the starting options.
- `STATE_GAME_RUNNING`: The main gameplay loop.
- `STATE_ANALYSIS_RUNNING`: The interactive replay and analysis mode.
- ... and others.

Each state corresponds to a function (e.g., `game_mode_display`, `analysis_mode_display`) that handles the logic for that specific mode and returns the next state.

### Move Validation
Move validation is primarily handled by `is_move_pattern_valid` in `chess.c`.
1. **Syntax Check**: `curr_move` parses the string input (e.g., "e2e4").
2. **Logic Check**: `is_move_pattern_valid` checks:
   - Is the piece moving according to its rules?
   - Is the path clear (for sliding pieces)?
   - Is the move a valid capture or quiet move?
   - Does the move leave the King in check? (This is often checked by simulating the move).

### Save/Load System
The game state is saved by serializing the linked list of `Board` nodes.
- **Saving**: `save_boards` traverses the tree of moves and writes each node's data (board state, connections) to a file.
- **Loading**: `load_boards` reads the file, reconstructs the nodes, and relinks them based on `id` and `previd`.

## Building and Development

To compile the project, ensure all source files are present and run:

```bash
gcc main.c chess.c display.c storage.c econio.c -o sakk
```

### Debugging
The project uses `debugmalloc.h`. To enable memory leak tracking, ensure it is included in `main.c`. It overrides standard `malloc`/`free` to track allocations.
