#include "chess.h"
#include "random.h"
#include "zobrist.h"

namespace ELSA
{

u64 Zobrist[SQUARE_NB][PIECE_NB];
u64 ZobristEnpass[FILE_NB];
u64 ZobristCastle[CASTLING_RIGHT_NB];
u64 ZobristTurn;

void initZobrist()
{
    for (int piece = W_PAWN; piece <= B_KING; ++piece)
        for (int sq = A1; sq <= H8; ++sq)
            Zobrist[sq][piece] = xorshift64();

    for (int f = 0; f < FILE_NB; f++)
        ZobristEnpass[f] = xorshift64();

    for (int cr = 0; cr < CASTLING_RIGHT_NB; cr++)
        ZobristCastle[cr] = xorshift64();

    ZobristTurn = xorshift64();
}

}; // namespace