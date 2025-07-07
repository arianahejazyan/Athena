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

> [!WARNING]
> The engine works but there is some configuration and installation issues, which will going to be fixed soon!


## 4PC Engines & Tools
<p align="justify"> 
Welcome to our growing list of four-player chess engines and tools! Here, you’ll find links to their GitHub repositories and Chess.com profiles, making it easy to explore and connect with others who share your passion. If you’ve developed a four-player chess engine, we warmly invite you to add it to this list. Sharing your work not only helps others get to know your engine and find it easily but also supports the growth of the four-player chess community. </p>

###  Engines 

<table align="center">
  <tr>
    <td>Athena (<a href="https://github.com/arianahejazyan/Athena" target="_blank">GitHub</a>, <a href="https://www.chess.com/member/teamathena1" target="_blank">Chess.com</a>)</td>
    <td>Samaritan (<a href="https://github.com/Moxile/Samaritan" target="_blank">GitHub</a>)</td>
  </tr>
</table>

### Tools
- Colosseum ([GitHub](https://github.com/arianahejazyan/Colosseum)) [GUI and tournamant manager]


## Licenses

> [!NOTE]
> The license texts included in the `licenses/` folder are snapshots as of **July 7, 2025**, and may not reflect the most recent upstream changes. For the latest versions, refer to the official repositories linked above.

- This project uses [CLI11](https://github.com/CLIUtils/CLI11) under the [BSD 3-Clause License](https://github.com/CLIUtils/CLI11/blob/main/LICENSE).
- This project uses [PyTorch](https://github.com/pytorch/pytorch) under the [BSD 3-Clause License](https://github.com/pytorch/pytorch/blob/main/LICENSE).
- This project uses [GoogleTest](https://github.com/google/googletest) under the [BSD 3-Clause License](https://github.com/google/googletest/blob/main/LICENSE).