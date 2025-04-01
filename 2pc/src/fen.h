#ifndef FEN_H
#define FEN_H

#include <string>
#include <sstream>
#include "bitboard.h"
#include "chess.h"
#include "types.h"
#include "position.h"

using namespace std;

namespace Talia
{

constexpr StaticArray<Castle, 256> ASCII_TO_CASTLING = []
{
    StaticArray<Castle, 256> table{};

    table['K'] = (Castle)(RIGHT & WHITESIDE & KINGSIDE);
    table['Q'] = (Castle)(RIGHT & WHITESIDE & QUEENSIDE);
    table['k'] = (Castle)(RIGHT & BLACKSIDE & KINGSIDE);
    table['q'] = (Castle)(RIGHT & BLACKSIDE & QUEENSIDE);

    return table;
}();

constexpr StaticArray<PieceType, 256> ASCII_TO_PIECE = []
{
    StaticArray<PieceType, 256> arr = {};

    arr['P'] = W_PAWN  ; arr['K'] = W_KING; arr['N'] = W_KNIGHT;
    arr['B'] = W_BISHOP; arr['R'] = W_ROOK; arr['Q'] = W_QUEEN;
    arr['p'] = B_PAWN  ; arr['k'] = B_KING; arr['n'] = B_KNIGHT;
    arr['b'] = B_BISHOP; arr['r'] = B_ROOK; arr['q'] = B_QUEEN;

    return arr;
}();

constexpr StaticArray<char, PIECE_TYPE_NB> PIECE_TO_ASCII = [] 
{
    StaticArray<char, PIECE_TYPE_NB> arr = {};

    arr[W_PAWN]   = 'P'; arr[W_KING] = 'K'; arr[W_KNIGHT] = 'N';
    arr[W_BISHOP] = 'B'; arr[W_ROOK] = 'R'; arr[W_QUEEN]  = 'Q';
    arr[B_PAWN]   = 'p'; arr[B_KING] = 'k'; arr[B_KNIGHT] = 'n';
    arr[B_BISHOP] = 'b'; arr[B_ROOK] = 'r'; arr[B_QUEEN]  = 'q'; arr[NO_PIECE] = ' ';

    return arr;
}();

[[nodiscard]] char toASCII(const PieceType type);

[[nodiscard]] bool fenIsValid(const string fen);

[[nodiscard]] Position fenToPosition(const string fen);

[[nodiscard]] string fenFromPosition(const Position& pos);

const string startpos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

} // namespace

#endif