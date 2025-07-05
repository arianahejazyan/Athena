<div align="center">

  <img src="./docs/assets/athena.png" alt="Athena Chess Engine Logo" style="width: 250px; height:250px; border-radius: 10px;"/>

  <h3>Athena</h3>

  A UCI-compatible four-player chess engine powered by deep RL and 256-bit bitboards.

</div>
<div align="center">
<img src="./docs/assets/boards/wallpaper.png" alt="Four Player Chess Board" style="width: 100%; height:auto; border-radius: 10px;"/>
</div>

## Table of Contents
- [Installation](#installation)
- [credits](#credits)


## Installation
Clone the repository

```bash
git clone https://github.com/arianahejazyan/Athena.git && cd Athena/athena 
```
Create a build directory and compile
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
```
Once built, you can run the Athena cli:
```bash
cd build && ./athena
```

## Credits

- This project uses the [CLI11](https://github.com/CLIUtils/CLI11) library for command-line parsing. ([BSD-3-Clause License](licenses/CLI11/LICENSE))