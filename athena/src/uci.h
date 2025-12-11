#pragma once

#include "chess.h"
#include <string>
#include <string_view>

namespace athena
{

PieceClass UCI2PieceClass(std::string_view pieceClass) noexcept;
Square UCI2Square(std::string_view square) noexcept;
Piece UCI2Piece(char piece) noexcept;
Color UCI2Color(char color) noexcept;

std::string PieceClass2UCI(PieceClass pieceClass) noexcept;
std::string square2UCI(Square square) noexcept;
std::string piece2UCI(Piece piece) noexcept;
std::string color2UCI(Color color) noexcept;

Move UCI2Move(std::string_view move) noexcept;
std::string move2UCI(Move move) noexcept;

} // namespace athena