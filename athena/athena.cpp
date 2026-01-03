#include <cstdint>
#include <iostream>
#include "cli.h"
#include "attacks.h"

int main(int argc, char *argv[])
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

    // athena::Bitboard bb(0,0,0,0);
    // bb |= athena::PEXT_DIAG[static_cast<uint8_t>(athena::Square::E2)][0x00];
    // bb.print();


    // athena::Bitboard bb(0,0,0,0);
    // bb.setSquare(athena::Square::F9);
    // bb.setSquare(athena::Square::J7);
    // bb.setSquare(athena::Square::K4);
    // bb.print();

    // auto x = athena::genBishopAttacks(athena::Square::I6, bb);
    // x.print();

    

    // auto z = athena::PEXT_TABLE_DIAG[static_cast<uint8_t>(athena::Square::F3)][0x01];
    // athena::Bitboard bb2(z, z, z, z);
    // bb2.print();



    // athena::Bitboard bb(athena::DIAGONAL[static_cast<int>(athena::Square::I6)].diag);
    // bb.print();

    // athena::Bitboard bb2(athena::DIAGONAL[static_cast<int>(athena::Square::I6)].anti);
    // bb2.print();


    // bb = bb.shift<athena::Offset::N>();
    // bb.print();
    
    athena::CLI cli;
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