<h1 align="center">august3</h1>

<p align="center">august3 is a UCI-compliant chess engine written in C, rated at ~2141 Elo using STS (Strategic Test Suite) rating.</p>

<p align="center"><img src="https://github.com/user-attachments/assets/e99726b5-670a-4281-86da-9d24652a084e" alt="board position example" width="800"/></p>

# Overview

This project is a minimal UCI‑compliant chess engine, built with the educational objective of exploring how chess engines function under the hood. I was specifically inspired by how Stockfish works.

By implementing the core components of a bitboard chess engine, I was able to have a deeper understanding of the algorithms, data structures, and design choices that is involved in modern chess engines.

august3 is able to communicate with standard chess interaces and tournament software using the UCI protocol. This allows the engine to be played against other chess engines or human players.

# Live Demo

![Animation](https://github.com/user-attachments/assets/76ba45bc-03b3-4aa1-85cc-3630d5d8c3f4)

Chess engine is displayed using Flask web app located here:
- Website: https://a3-engine.onrender.com
- Repo: https://github.com/r5lzr/web-engine

# Features

- Bitboard board representation as a 64-bit bitmap
- Pre-calculated attack tables for leaper pieces
- Magic bitboards for sliding pieces
- Legal move generation encoding with perft testing
- Make/Unmake approach
- Negamax search with alpha-beta pruning
- Quiescence search
- Iterative deepening
- Principle variation search
- PV/killer/history move ordering
- Transposition table with Zobrist keys
- Repetition detection
- UCI protocol

# Installation

august3 can be built for both Windows and Unix systems. The respective binary executable files are already given in the Releases section of this repo, however if you would like to recompile the builds, the instructions are below:

## Requirements

- Windows: gcc-14.2.0-mingw-w64ucrt-12.0.0-r2
- Unix: gcc 9.4.0/11.4.0
- make build system

### Windows:
```
cd august3
make -f makefile-windows clean
make -f makefile-windows all
```

### Unix:
```
cd august3
make -f makefile-unix clean
make -f makefile-unix all
```

# Usage

august3 can be launched from its respective binary executable files in a UCI GUI or from the command line.

## UCI GUI
Open the engine from the UCI GUI software such as [Arena](http://www.playwitharena.de/) and install new chess engine pointing towards the august3 executable file. Make sure to choose the 'Release' file for full performance.

## UCI CLI
### Windows:
```
cd august3
./bin/Windows/Release/august3
```

### Unix:
```
cd august3
./bin/Unix/Release/august3
```

### Example UCI commands:
```bash
uci # initialize protocol
isready  # wait until engine responds ready
position startpos moves e2e4 e7e5 # from starting board, move white to e2e4 and black to e7e5
go wtime 300000 btime 300000 # finds best move for given time in ms, in this case for white
stop # stop searching and display best move
quit # exit out of engine
```

# License

Distributed under the GPL-3.0 License. See `LICENSE` for more information.
