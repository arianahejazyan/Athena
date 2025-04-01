#ifndef BOARD_H
#define BOARD_H

#include <cassert>
#include "bitboard.h"
#include "chess.h"
#include "types.h"

namespace Talia
{

struct Board
{
    private:

        StaticArray<PieceType, SQUARE_NB> mailbox;
        StaticArray<BitBoard, COLOR_NB> occupied;
        StaticArray<BitBoard, PIECE_NB> bitboard;
    
    public:

        Board()  = default;
        ~Board() = default;

        inline void setAllZero()
        {
            mailbox.fill(NO_PIECE_TYPE);
            occupied.fill(EMPTY_BOARD);
            bitboard.fill(EMPTY_BOARD);
        }

        [[nodiscard]] inline PieceType at(Square sq) const
        {
            assert(A1 <= sq && sq <= H8);

            return mailbox[sq];
        }

        [[nodiscard]] inline BitBoard occ(const Piece piece, const Color color) const
        {
            assert(PAWN  <= piece && piece <= KING);
            assert(WHITE <= color && color <= BLACK);

            return bitboard[piece] & occupied[color];
        }

        [[nodiscard]] inline BitBoard occ(const PieceType type) const
        {
            assert(W_PAWN <= type && type <= B_KING);

            return bitboard[toPiece(type)] & occupied[toColor(type)];
        }

        [[nodiscard]] inline BitBoard occ(const Piece piece) const
        {
            assert(PAWN <= piece && piece <= KING);

            return bitboard[piece];
        }

        [[nodiscard]] inline BitBoard occ(const Color color) const
        {
            assert(WHITE <= color && color <= BLACK);

            return occupied[color];
        }

        [[nodiscard]] inline BitBoard occ() const
        {
            return occupied[WHITE] | occupied[BLACK];
        }

        inline void setSQ(const Square sq, const Piece piece, const Color color)
        {
            assert(PAWN  <= piece && piece <= KING);
            assert(WHITE <= color && color <= BLACK);
            assert(A1 <= sq && sq <= H8);

            mailbox[sq] = toPieceType(piece, color); 

            setbb(bitboard[piece], sq);
            setbb(occupied[color], sq);
        }

        inline void setSQ(const Square sq, const PieceType type)
        {
            assert(W_PAWN <= type && type <= B_KING);
            assert(A1 <= sq && sq <= H8);
            
            mailbox[sq] = type;

            setbb(bitboard[toPiece(type)], sq);
            setbb(occupied[toColor(type)], sq);
        }

        inline void popSQ(const Square sq)
        {
            assert(A1 <= sq && sq <= H8);

            const PieceType type = mailbox[sq];

            mailbox[sq] = NO_PIECE_TYPE;

            popbb(bitboard[toPiece(type)], sq);
            popbb(occupied[toColor(type)], sq);
        }
};

} // namespace

#endif