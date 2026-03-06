#include <cstddef>
#include <cstdint>
#include <iostream>
#include "cli.h"
#include "attacks.h"
#include "src/attacks.h"
#include "src/bitboard.h"
#include "src/constants.h"
#include "src/fen.h"
#include "src/position.h"
#include "src/square.h"
#include "movegen.h"

using namespace athena;

int main(int argc, char *argv[]) // ccheck bitboard temperoray
{
    if (argc > 1)
    {
        std::string name = argv[1];
        
        if (name == "--version")
        {
            std::cout << "Athena Engine CLI v0.0.0" << std::endl;
            return 0;
        }
    
        if (name == "--help")
        {
            std::cout << "help message" << std::endl;
            return 0;
        }

        std::cout << "info string unknown flag '" << name << "'" << std::endl;
        return 1;
    }

    Position pos;
    pos.init(STARTPOS);
    pos.print();

    pos.movegen();
    Move moves[MOVE_NB];
    std::size_t n = 0;
    n += generate_noisy_moves(pos, moves + n, GameSetup::Modern);
    n += generate_quiet_moves(pos, moves + n, GameSetup::Modern);
    std::cout << n << std::endl;

    for (std::size_t i = 0; i < n; i++)
    {
        std::cout << "["<<i<<"] " << moves[i].uci(true) << std::endl;
    }

    // auto x = homerank_bitboard(Color::Red);
    // x.print();

    // Square sq = Square::E2;
    // Bitboard occ(0);
    // occ |= rank_bitboard(1);
    // occ |= rank_bitboard(2);
    // occ |= rank_bitboard(13);
    // occ |= rank_bitboard(14);
    // occ |= file_bitboard(1);
    // occ |= file_bitboard(2);
    // occ |= file_bitboard(13);
    // occ |= file_bitboard(14);
    // // occ.set(sq);
    // // occ.set(Square::J7);
    // // occ.set(Square::H10);
    // // occ.set(Square::H3);
    // Bitboard attacks = rook_attacks(sq, occ);
    // occ.print();
    // attacks.print();

    CLI cli;
    cli.launch();

    return 0;
}

// R-0,0,0,0-1,1,1,1-1,1,1,1-0,0,0,0-0-{'enPassant':('','h3:','','')}-
// x,x,x,yR,yN,yB,yK,yQ,yB,yN,yR,x,x,x/
// x,x,x,yP,yP,yP,yP,yP,yP,yP,yP,x,x,x/
// x,x,x,8,x,x,x/
// bR,bP,10,gP,gR/
// bN,bP,10,gP,gN/
// bB,bP,10,gP,gB/
// bQ,bP,10,gP,gK/
// bK,bP,10,gP,gQ/
// bB,bP,10,gP,gB/
// bN,bP,10,gP,1/
// bR,bP,11,gR/
// x,x,x,8,x,x,x/
// x,x,x,3,rP,1,rP,2,x,x,x/
// x,x,x,8,x,x,x

// R-0,0,0,0-1,1,1,1-1,1,1,1-0,0,0,0-0-{'enPassant':('','h11:','','')}-
// x,x,x,yR,yN,yB,yK,yQ,yB,yN,yR,x,x,x/
// x,x,x,yP,yP,yP,yP,yP,yP,yP,yP,x,x,x/
// x,x,x,8,x,x,x/
// bR,bP,10,gP,gR/
// bN,bP,4,rP,1,rP,3,gP,gN/
// bB,bP,10,gP,gB/
// bQ,bP,10,gP,gK/
// bK,bP,10,gP,gQ/
// bB,bP,10,gP,gB/
// bN,bP,10,gP,1/
// bR,bP,11,gR/
// x,x,x,8,x,x,x/
// x,x,x,8,x,x,x/
// x,x,x,8,x,x,x