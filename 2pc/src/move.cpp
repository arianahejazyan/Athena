#include "move.h"
#include "bitboard.h"

namespace Talia
{

[[nodiscard]] std::string Move::toUCI() const noexcept
{
    std::string uci_move;

    uci_move += ('a' + (fileSQ(source())));
    uci_move += ('1' + (rankSQ(source())));
    uci_move += ('a' + (fileSQ(target())));
    uci_move += ('1' + (rankSQ(target())));

    switch (nature())
    {
        case FLAG_QUEEN:  uci_move += 'q'; break;
        case FLAG_ROOK:   uci_move += 'r'; break;
        case FLAG_BISHOP: uci_move += 'b'; break;
        case FLAG_KNIGHT: uci_move += 'n'; break;
    
        default: break;
    }

    return uci_move;
}

} // namespace