# C Programming Practice

A collection of terminal-based C exercises, small games and Russian-language learning notes.

This is a **learning repository**, not a production application. The source code and walkthroughs are kept together so that each project can be read, compiled and explored independently.

## Projects

| Project | Source | Description |
| --- | --- | --- |
| Conway's Game of Life | [game_of_life.c](game_of_life.c) | Terminal simulation with an 80×25 grid, preset initial states and an ncurses interface. |
| Pong | [pong.c](pong.c) | A two-player console game with paddle movement, ball physics and scoring. |

Russian-language guides and task notes are available in the root directory, including [the Game of Life walkthrough](game_of_life.md) and [the Pong guide](pong_guide.md).

## Build

Requires a C compiler; Game of Life additionally requires the ncurses development library.

```bash
cc -std=c11 -Wall -Wextra -O2 game_of_life.c -lncurses -o game_of_life
cc -std=c11 -Wall -Wextra -O2 pong.c -o pong
```

The `initial_state_*.txt` files contain sample starting layouts for Game of Life. The repository also includes a prebuilt `game_of_life` binary, but compiling the source on your own system is preferable.

## Repository purpose

These exercises document C fundamentals, terminal I/O, state updates and small interactive programs. The accompanying notes are primarily in Russian.
