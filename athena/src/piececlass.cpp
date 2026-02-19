#include "piececlass.h"

namespace athena
{

PieceClass::PieceClass(std::string_view pc) noexcept
{
    Color color = Color(pc[0]);
    Piece piece = Piece(pc[1]);

    encoded_ = (static_cast<uint8_t>(color) << 3) | static_cast<uint8_t>(piece);
}

std::string PieceClass::uci() const noexcept
{
    std::string str = "";
    str += color().uci();
    str += piece().uci();
    return str;
}

} // namespace athena