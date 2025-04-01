#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "types.h"
#include "chess.h"

namespace Talia
{

struct GameState
{
    // Color turn;
    u64 hash;
    Square epsq;
    Castle castle;
    PieceType captured;
    int halfmove;
    int fullmove;

    GameState()  = default;
    ~GameState() = default;
};

} // namespace

#endif