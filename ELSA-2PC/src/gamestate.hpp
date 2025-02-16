#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "chess.hpp"
#include "types.hpp"

namespace ELSA
{

struct GameState
{
    PieceType mailbox[BOARD_SIZE];
    u64 occupied[PIECE_SIZE];
};

}; // namespace

#endif