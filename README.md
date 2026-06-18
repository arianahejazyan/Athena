<div align="center">

  <img src="./docs/assets/logo.png" alt="Athena Chess Engine Logo" style="width: 250px; height:250px; border-radius: 10px;"/>

  <h3>Athena</h3>

  A UCI-compatible four-player chess engine

</div>
<div align="center">
<img src="./docs/assets/wallpaper.png" alt="Four Player Chess Board" style="width: 100%; height:auto; border-radius: 10px;"/>
</div>

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
    <td>Enigma (<a href="https://github.com/anurag-baundwal/4pchess" target="_blank">GitHub</a>, <a href="https://www.chess.com/member/teamenigma1" target="_blank">Chess.com</a>)</td>
  </tr>
  <tr>
  <td>Nexus (<a href="https://www.chess.com/member/TeamNexus1" target="_blank">Chess.com</a>)</td>
  <td>BlackKnight (<a href="https://www.chess.com/member/TeamBlackKnight1" target="_blank">Chess.com</a>)</td>
  </tr>
</table>

You can also find four-player chess tools list in this [repo](https://github.com/arianahejazyan/Colosseum).

## Acknowledgements
- **Special thanks** to Moxile for always supporting me and for the many hours of programming together.
- **Special thanks** to tsoj, Lotfy and the Discord chess engine community for their help with coding and discussions.
- **Special thanks** to qilp and Chess.com team for providing API access and support.
