#ifndef ZOBRIST_H
#define ZOBRIST_H

// #include "chess.h"

namespace Talia
{

// // extern u64 Zobrist[SQUARE_NB][PIECE_NB];
// // extern u64 ZobristEnpass[FILE_NB];
// // extern u64 ZobristCastle[CASTLING_RIGHT_NB];
// // extern u64 ZobristTurn;

// // void initZobrist();

} // namespace

#endif


// u64 Zobrist[SQUARE_NB][PIECE_NB];
// u64 ZobristEnpass[FILE_NB];
// u64 ZobristCastle[CASTLING_RIGHT_NB];
// u64 ZobristTurn;

// void initZobrist()
// {
//     for (int piece = W_PAWN; piece <= B_KING; ++piece)
//         for (int sq = A1; sq <= H8; ++sq)
//             Zobrist[sq][piece] = xorshift64();

//     for (int f = 0; f < FILE_NB; f++)
//         ZobristEnpass[f] = xorshift64();

//     for (int cr = 0; cr < CASTLING_RIGHT_NB; cr++)
//         ZobristCastle[cr] = xorshift64();

//     ZobristTurn = xorshift64();
// }

// // static void initStateHash(GameState& state, Info& si)
// // {
// //     for (int s = A1; s <= H8; s++)
// //         if (state.at(static_cast<Square>(s)) != NO_PIECE) si.hash ^= Zobrist[s][state.at(static_cast<Square>(s))];

// //     si.hash ^= ZobristCastle[si.castle];

// //     if (si.epsq != OFFBOARD) si.hash ^= ZobristEnpass[getFile(si.epsq)];
// //     if (si.turn == BLACK)    si.hash ^= ZobristTurn;
// // }