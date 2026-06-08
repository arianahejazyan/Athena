#include "move.h"

namespace athena
{

std::string Move::uci(bool board16x16) const noexcept
{
    std::string str = "";
    str += source().uci(board16x16);
    str += target().uci(board16x16);
    if (type() == MoveType::Evolve) {
        str += evolve().uci();
    }
    return str;
}

} // namespace athena