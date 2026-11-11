#pragma once

#include "chess/constants.h"
#include <cstdint>
#include <string>

namespace athena::utils {

struct FENBin {
    uint8_t pieces[chess::BOARD_SIZE[chess::BoardRepresentation::ChessCom]];
    uint8_t side_to_move; 
    uint8_t castling;
    uint8_t fifty_move_clock;
    uint8_t ep_square[7];
};

class FEN {
public:
    struct BIN {
        char pieces[196];
        char stm;
        char castle;
        char fiftymoveclock;
    };

private:
    std::string fen_;
};


// inline static const std::array<FEN, SETUP_NB> STARTPOS = 
// {
//     FEN{
//         "R-0,0,0,0-1,1,1,1-1,1,1,1-0,0,0,0-0-"
//         "x,x,x,yR,yN,yB,yK,yQ,yB,yN,yR,x,x,x/"
//         "x,x,x,yP,yP,yP,yP,yP,yP,yP,yP,x,x,x/"
//         "x,x,x,8,x,x,x/"
//         "bR,bP,10,gP,gR/"
//         "bN,bP,10,gP,gN/"
//         "bB,bP,10,gP,gB/"
//         "bQ,bP,10,gP,gK/"
//         "bK,bP,10,gP,gQ/"
//         "bB,bP,10,gP,gB/"
//         "bN,bP,10,gP,gN/"
//         "bR,bP,10,gP,gR/"
//         "x,x,x,8,x,x,x/"
//         "x,x,x,rP,rP,rP,rP,rP,rP,rP,rP,x,x,x/"
//         "x,x,x,rR,rN,rB,rQ,rK,rB,rN,rR,x,x,x"
//     },

//     FEN{
//         "R-0,0,0,0-1,1,1,1-1,1,1,1-0,0,0,0-0-"
//         "x,x,x,yR,yN,yB,yK,yQ,yB,yN,yR,x,x,x/"
//         "x,x,x,yP,yP,yP,yP,yP,yP,yP,yP,x,x,x/"
//         "x,x,x,8,x,x,x/"
//         "bR,bP,10,gP,gR/"
//         "bN,bP,10,gP,gN/"
//         "bB,bP,10,gP,gB/"
//         "bK,bP,10,gP,gQ/"
//         "bQ,bP,10,gP,gK/"
//         "bB,bP,10,gP,gB/"
//         "bN,bP,10,gP,gN/"
//         "bR,bP,10,gP,gR/"
//         "x,x,x,8,x,x,x/"
//         "x,x,x,rP,rP,rP,rP,rP,rP,rP,rP,x,x,x/"
//         "x,x,x,rR,rN,rB,rQ,rK,rB,rN,rR,x,x,x"
//     }
// };

// public:
// static std::string startpos(const std::string& setup = "modern") noexcept {
//     return STARTPOS[setup == "modern" ? 0 : 1];
// }

} // namespace athena
