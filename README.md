<div align="center">

  <img src="./docs/assets/logo.png" alt="Athena Chess Engine Logo" style="width: 250px; height:250px; border-radius: 10px;"/>

  <h3>Athena</h3>

  A UCI-compatible four-player chess engine

</div>
<div align="center">
<img src="./docs/assets/wallpaper.png" alt="Four Player Chess Board" style="width: 100%; height:auto; border-radius: 10px;"/>
</div>

## Getting Started
### Clone & Build

```bash
git clone https://github.com/arianahejazyan/Athena.git
cd Athena
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

*Prerequisites*:
* A C++ compiler with C++23 support
* CMake as the configuration system
* A build system such as Make or Ninja
* GoogleTest library

### Command

Once built, launch the interactive Athena engine cli:

```bash
./build/athena
```

> See [manual](./docs/manual.md) for the full list of commands and options.

### Tests

**Perft** counts nodes at a given depth to validate move generation.

```bash
./build/tests/perft [options]
```

*Options*:
* `--modern`: Start from the modern chess starting position
* `--classic`: Start from the classic chess starting position
* `--fen <fen>`: Start from a custom FEN position
* `--depth <depth>`: Set the search depth
* `--split`: Show per-move node counts at the root
* `--setup <modern|classic>`: Set the castling setup variant

*Example*:
```bash
./build/tests/perft --modern --depth 6
```

## 4PC Engines
<p align="justify"> 
This is a list of four-player chess engines to help discover and support other engines. Feel free to add your chess engine here, or ask me to add it for you. You can include your Chess.com profile and GitHub repository. </p>
 
<table align="center">
  <thead>
    <tr>
      <th>Engine</th>
      <th>GitHub</th>
      <th>Chess.com</th>
    </tr>
  </thead>
  <tbody>
    <tr>
      <td>Athena</td>
      <td><a href="https://github.com/arianahejazyan/Athena" target="_blank">arianahejazyan/Athena</a></td>
      <td><a href="https://www.chess.com/member/teamathena1" target="_blank">TeamAthena</a></td>
    </tr>
    <tr>
      <td>Samaritan</td>
      <td><a href="https://github.com/Moxile/Samaritan" target="_blank">Moxile/Samaritan</a></td>
      <td>—</td>
    </tr>
    <tr>
      <td>Enigma</td>
      <td><a href="https://github.com/anurag-baundwal/4pchess" target="_blank">anurag-baundwal/4pchess</a></td>
      <td><a href="https://www.chess.com/member/teamenigma1" target="_blank">TeamEnigma</a></td>
    </tr>
    <tr>
      <td>Nexus</td>
      <td>(private)</td>
      <td><a href="https://www.chess.com/member/TeamNexus1" target="_blank">TeamNexus</a></td>
    </tr>
    <tr>
      <td>BlackKnight</td>
      <td>(private)</td>
      <td><a href="https://www.chess.com/member/TeamBlackKnight1" target="_blank">TeamBlackKnight</a></td>
    </tr>
    <tr>
      <td>Titan</td>
      <td><a href="https://github.com/obryanlouis/4pchess" target="_blank">obryanlouis/4pchess</a></td>
      <td><a href="https://www.chess.com/member/teamtitan1" target="_blank">TeamTitan</a></td>
    </tr>
    <tr>
      <td>Terminator</td>
      <td>(private)</td>
      <td><a href="https://www.chess.com/member/teamterminator1" target="_blank">TeamTerminator</a></td>
    </tr>
  </tbody>
</table>

You can also find four-player chess tools list in this [repo](https://github.com/arianahejazyan/Colosseum).

## Acknowledgements
- **Special thanks** to Moxile for always supporting me and for the many hours of programming together.
- **Special thanks** to tsoj, Lotfy and the Discord chess engine community for their help with coding and discussions.
- **Special thanks** to qilp and Chess.com team for providing API access and support.
