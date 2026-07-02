<div align="center">

  <img src="./assets/logo.png" alt="Athena Chess Engine Logo" style="width: 250px; height:250px; border-radius: 10px;"/>

  <h3>Athena</h3>

  A UCI-compatible four-player chess engine

</div>
<div align="center">
<img src="./assets/wallpaper.png" alt="Four Player Chess Board" style="width: 100%; height:auto; border-radius: 10px;"/>
</div>

## Table of Contents
*coming soon*
## Introduction
*coming soon*
## Getting Started
To build Athena from source, run the following commands:
```bash
git clone https://github.com/arianahejazyan/Athena.git
cd Athena
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```
Launch the interactive CLI with:
```
./build/src/athena
```

Athena's move generator is highly optimized, achieving approximately 120 Mnps (million nodes per second) in benchmarks. You can verify this yourself:
```bash
./build/tests/perft_bench --benchmark_counters_tabular=true
```

> See the [commands](#Commands) section for the full list of commands and options.

## Commands

Athena is a UCI-compatible chess engine and supports all standard UCI commands.

### UCI Commands

| Command       | Description                                                                 |
|---------------|-----------------------------------------------------------------------------|
| `uci`         | Identifies the engine and returns its name, version, and supported options. |
| `isready`     | Checks if the engine is ready; responds with `readyok` when synchronized.   |
| `setoption`   | Sets a configuration option (e.g. `Hash`, `Threads`).                      |
| `ucinewgame`  | Notifies the engine that a new game is about to begin.                      |
| `position`    | Sets the board position, optionally followed by a sequence of moves.        |
| `go`          | Starts the search on the current position.                                  |
| `stop`        | Stops the current search as soon as possible.                               |
| `quit`        | Shuts down the engine.                                                      |

### UCI Options

| Name    | Default  | Description                                                  |
|---------|----------|--------------------------------------------------------------|
| `Setup` | `modern` | Board setup variant to use (`modern` or `classic`).          |

### Debug Commands

Athena also provides additional commands for testing and debugging:

| Command | Description                                                                      |
|---------|----------------------------------------------------------------------------------|
| `perft` | Runs a perft (performance test) to count legal moves at a given depth.           |
| `print` | Displays the current board position in the console.

## Features
### Chess
*coming soon*
### Search
*coming soon*
### Evaluate
*coming soon*
## 4PC Engines
This is a list of active four-player chess (4PC) engines. Feel free to add your own engine here or ask me to include it. You can also find a list of four-player chess tools in the [Colosseum](https://github.com/arianahejazyan/Colosseum).
 
| Engine        | GitHub                                      | Chess.com                          |
|---------------|---------------------------------------------|------------------------------------|
| Athena        | [arianahejazyan/Athena](https://github.com/arianahejazyan/Athena) | [TeamAthena](https://www.chess.com/member/teamathena1) |
| Samaritan     | [Moxile/Samaritan](https://github.com/Moxile/Samaritan) | —                                  |
| Enigma        | [anurag-baundwal/4pchess](https://github.com/anurag-baundwal/4pchess) | [TeamEnigma](https://www.chess.com/member/teamenigma1) |
| Nexus         | (private)                                   | [TeamNexus](https://www.chess.com/member/TeamNexus1) |
| BlackKnight   | (private)                                   | [TeamBlackKnight](https://www.chess.com/member/TeamBlackKnight1) |
| Titan         | [obryanlouis/4pchess](https://github.com/obryanlouis/4pchess) | [TeamTitan](https://www.chess.com/member/teamtitan1) |
| Terminator    | (private)                                   | [TeamTerminator](https://www.chess.com/member/teamterminator1) |

## Acknowledgements
- **Special thanks** to Moxile for always supporting me and for the many hours of programming together.
- **Special thanks** to tsoj, Lotfy and the Discord chess engine community for their help with coding and discussions.
- **Special thanks** to qilp and Chess.com team for providing API access and support.
- **Special thanks** to all contributors for their support and contributions.