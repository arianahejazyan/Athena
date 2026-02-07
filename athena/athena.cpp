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