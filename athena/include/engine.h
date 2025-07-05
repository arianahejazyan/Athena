#ifndef ENGINE_H
#define ENGINE_H

#include <CLI/CLI.hpp>
#include <iostream>
#include <cstring>
#include "chess.h"
#include "position.h"

namespace athena
{

constexpr auto FEN_MODERN  =  "modern R 0 1111 1111 -,-,-,- rr,rn,rb,rq,rk,rb,rn,rr,rp,rp,rp,rp,rp,rp,rp,rp,8,br,bp,10,gp,gr,bn,bp,10,gp,gn,bb,bp,10,gp,gb,bk,bp,10,gp,gq,bq,bp,10,gp,gk,bb,bp,10,gp,gb,bn,bp,10,gp,gn,br,bp,10,gp,gr,8,yp,yp,yp,yp,yp,yp,yp,yp,yr,yn,yb,yk,yq,yb,yn,yr";
constexpr auto FEN_CLASSIC = "classic R 0 1111 1111 -,-,-,- rr,rn,rb,rq,rk,rb,rn,rr,rp,rp,rp,rp,rp,rp,rp,rp,8,br,bp,10,gp,gr,bn,bp,10,gp,gn,bb,bp,10,gp,gb,bq,bp,10,gp,gk,bk,bp,10,gp,gq,bb,bp,10,gp,gb,bn,bp,10,gp,gn,br,bp,10,gp,gr,8,yp,yp,yp,yp,yp,yp,yp,yp,yr,yn,yb,yk,yq,yb,yn,yr";

class Engine
{
    private:

        Position pos;
        CLI::App app{"Athena Engine CLI"};

        // Configuration
        bool debug = false;

        // Position options
        std::string position_mode;

        // Perft options
        int  perft_depth;
        bool perft_full;
        bool perft_split;
        bool perft_cumulative;

        // UCI commands
        void handleUCI();
        void handleIsReady();
        void handleSetOption();
        void handleUCINewGame();
        void handlePosition();
        void handleGo();
        void handleStop();
        void handleQuit();

        // Other commands
        void handlePerft();
        void handlePrint();
        void handleConfig();
        
    public:

        Engine();

        void launch();
        void execute(int argc, const char* argv[]);
};

} // namespace athena

#endif // #ifndef ENGINE_H