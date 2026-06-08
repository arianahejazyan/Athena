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
- [4PC engines and tools](#4pc-engines--tools)
  - engines
  - tools
- [licenses](#licenses)


## Installation
Clone the repository

```bash
git clone https://github.com/arianahejazyan/Athena.git && cd Athena/athena 
```
Create a build directory and compile
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
```
Once built, you can run the Athena cli
```bash
cd build && ./athena
```

## 4PC Engines
<p align="justify"> 
This is a list of four-player chess engines to help discover and support other engines. Feel free to add your chess engine here, or ask me to add it for you. You can include your Chess.com profile and GitHub repository. </p>
 
<table align="center">
  <tr>
    <td>Athena (<a href="https://github.com/arianahejazyan/Athena" target="_blank">GitHub</a>, <a href="https://www.chess.com/member/teamathena1" target="_blank">Chess.com</a>)</td>
    <td>Samaritan (<a href="https://github.com/Moxile/Samaritan" target="_blank">GitHub</a>)</td>
  </tr>
</table>



You can also find four-player chess tools list in this [repo](https://github.com/arianahejazyan/Colosseum).

## Licenses

> [!NOTE]
> <p align="justify">The license texts included in the <b>licenses</b> folder are snapshots as of <b>July 7, 2025</b>, and may not reflect the most recent upstream changes. For the latest versions, refer to the official repositories linked below.</p>

- This project uses [CLI11](https://github.com/CLIUtils/CLI11) under the [BSD 3-Clause License](https://github.com/CLIUtils/CLI11/blob/main/LICENSE).
- This project uses [PyTorch](https://github.com/pytorch/pytorch) under the [BSD 3-Clause License](https://github.com/pytorch/pytorch/blob/main/LICENSE).
- This project uses [Benchmark](https://github.com/google/benchmark) under the [Apache 2.0 License](https://github.com/google/benchmark/blob/main/LICENSE).
- This project uses [GoogleTest](https://github.com/google/googletest) under the [BSD 3-Clause License](https://github.com/google/googletest/blob/main/LICENSE).