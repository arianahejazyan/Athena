#include "chess.h"

namespace ELSA
{

PieceType getPieceType(Piece piece)
{
    return (PieceType)(piece <= W_KING ? piece : piece - 6);
}

Player getPlayer(Piece piece)
{
    return (Player)(piece <= W_KING);
}

}; // namespace