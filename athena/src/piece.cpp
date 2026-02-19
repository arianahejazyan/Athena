#include "piece.h"

namespace athena
{

Piece::Piece(std::string_view piece) noexcept
{
    switch (piece[0])
    {
        case 'P': value_ = Piece::Pawn  ; break;
        case 'N': value_ = Piece::Knight; break;
        case 'B': value_ = Piece::Bishop; break;
        case 'R': value_ = Piece::Rook  ; break;
        case 'Q': value_ = Piece::Queen ; break;
        case 'K': value_ = Piece::King  ; break;
        default : value_ = Piece::Empty ; break;
    }
}

std::string Piece::uci() const noexcept
{
    switch (value_)
    {
        case Piece::Pawn  : return "P";
        case Piece::Knight: return "N";
        case Piece::Bishop: return "B";
        case Piece::Rook  : return "R";
        case Piece::Queen : return "Q";
        case Piece::King  : return "K";
        default           : return "" ;
    }
}

} // namespace athena