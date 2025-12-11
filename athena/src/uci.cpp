#include "uci.h"

namespace athena
{

PieceClass UCI2PieceClass(std::string_view pieceClass) noexcept
{
    Color color = UCI2Color(pieceClass[0]);
    Piece piece = UCI2Piece(pieceClass[1]);

    return PieceClass(color, piece);
}

Square UCI2Square(std::string_view square) noexcept
{
    int rank = std::stoi(std::string(square.substr(1))); 
    int file = square[0] - 'a' + 1;

    return Square(rank * FILE_NB + file);
}

Piece UCI2Piece(char piece) noexcept
{
    switch (piece)
    {
        case 'P': return Piece::Pawn  ;
        case 'N': return Piece::Knight;
        case 'B': return Piece::Bishop;
        case 'R': return Piece::Rook  ;
        case 'Q': return Piece::Queen ;
        case 'K': return Piece::King  ;
        default : return Piece::Empty ;
    }
}

Color UCI2Color(char color) noexcept
{
    switch (color)
    {   
        case 'r': return Color::Red   ;
        case 'b': return Color::Blue  ;
        case 'y': return Color::Yellow;
        case 'g': return Color::Green ;
        default : return Color::None  ;
    }
}

std::string PieceClass2UCI(PieceClass pc) noexcept
{
    std::string str = "";
    str += color2UCI(pc.color());
    str += piece2UCI(pc.piece());
    return str;
}

std::string square2UCI(Square square) noexcept
{
    if (square == Square::Offboard)
        return "-";

    return std::string(1, static_cast<char>('a' + file(square) - 1)) + std::to_string(rank(square));
}

std::string piece2UCI(Piece piece) noexcept
{
    switch (piece)
    {
        case Piece::Pawn  : return "P";
        case Piece::Knight: return "N";
        case Piece::Bishop: return "B";
        case Piece::Rook  : return "R";
        case Piece::Queen : return "Q";
        case Piece::King  : return "K";
        default           : return "?";
    }
}

std::string color2UCI(Color color) noexcept
{
    switch (color)
    {   
        case Color::Red   : return "r";
        case Color::Blue  : return "b";
        case Color::Yellow: return "y";
        case Color::Green : return "g";
        default           : return "?";
    }
}

std::string move2UCI(Move move) noexcept
{
    std::string str = "";
    Square source = move.source();
    Square target = move.target();

    str += square2UCI(source);
    str += square2UCI(target);

    MoveType type = move.type();

    if (type == MoveType::Evolve)
    {
        str += std::tolower(piece2UCI(move.evolve())[0]);
    }

    return str;
}

} // namespace athena