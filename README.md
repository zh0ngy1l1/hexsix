# Hexsix Tic-Tac-Toe

A small C++17 Tic-Tac-Toe game built with SFML 3.

## Features
- 3x3 Tic-Tac-Toe board
- Mouse click input
- Turn, win, and draw status text
- Restart with `R`
- Quit with `Esc`

## Requirements
- CMake 3.16+
- C++17 compiler
- SFML 3 (`Graphics`, `Window`, `System`)

## Build
```bash
cmake -S . -B build
cmake --build build
```

## Run
```bash
./build/hexsix
```

## Controls
- Left click: place mark
- `R`: restart game
- `Esc`: quit

## Project Structure
- `main.cpp`: application entry point
- `game.*`: pure game rules and state
- `render.*`: SFML windowing, input, and drawing
