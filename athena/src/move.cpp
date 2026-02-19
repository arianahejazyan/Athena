#include "move.h"

namespace athena
{

std::string Move::uci() const noexcept
{
    std::string str = "";
    str += source().uci();
    str += target().uci();
    str += evolve().uci();
    return str;
}

} // namespace athena