#ifndef ZOBRIST_H
#define ZOBRIST_H

#include "chess.h"

namespace ELSA
{

extern u64 Zobrist[SQUARE_NB][PIECE_NB];
extern u64 ZobristEnpass[FILE_NB];
extern u64 ZobristCastle[CASTLING_RIGHT_NB];
extern u64 ZobristTurn;

void initZobrist();

}; // namespace

#endif