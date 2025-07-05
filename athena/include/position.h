#ifndef POSITION_H
#define POSITION_H

#include "bitboard.h"
#include "chess.h"

namespace athena
{

class Board
{
    private:

        ndarray<PieceClass, SQUARE_NB> mailbox;
        ndarray<BitBoard, PIECE_NB> pieces;
        ndarray<BitBoard, COLOR_NB> colors;

    public:

        Board() noexcept { clear(); }

        ~Board() noexcept = default;

        inline auto operator[](Square sq) const noexcept {
            return mailbox[sq];
        }

        inline auto occ(Color color) const noexcept {
            return colors[color];
        }

        inline auto occ(Piece piece) const noexcept {
            return pieces[piece];
        }

        inline auto occ(Piece p1 , Piece p2) const noexcept {
            return pieces[p1] | pieces[p2];
        }

        inline auto occ(Piece piece , Color color) const noexcept {
            return pieces[piece] & colors[color];
        }

        inline auto opponent(Color color) const noexcept {
            return colors[OPPONENTS[color][0]] | colors[OPPONENTS[color][1]] ;
        }
    
        inline auto teammate(Color color) const noexcept {
            return colors[TEAMMATES[color][0]] | colors[TEAMMATES[color][1]] ;
        }

        inline auto everyone() const noexcept {
            return colors[Red] | colors[Blue] | colors[Yellow] | colors[Green];
        }

        void clear() noexcept 
        {
            for (auto sq : ALL_SQUARES)
                mailbox[sq] = isValidSquare(rankSQ(sq), fileSQ(sq)) ? EMPTY : STONE;
            pieces.fill(BB{});
            colors.fill(BB{});
        }

        inline void setSQ(Square sq, PieceClass pc) noexcept
        {
            mailbox[sq] = pc;
            pieces[pc.piece()].setSQ(sq);
            colors[pc.color()].setSQ(sq);
        }

        inline void popSQ(Square sq) noexcept
        {
            auto pc = mailbox[sq];
            mailbox[sq] = EMPTY;
            pieces[pc.piece()].popSQ(sq);
            colors[pc.color()].popSQ(sq);
        }

        inline auto royal(Color color) const noexcept {
            return (pieces[King] & colors[color]).lsb();
        }
};

class GameState
{
    public:

        int clock;
        Color turn;
        BitBoard hash;
        BitBoard castle;
        PieceClass captured;
        ndarray<Square, COLOR_NB - 1> enpass;

        GameState()  noexcept = default;
        ~GameState() noexcept = default;

        GameState
        (
            int clock_,
            Color turn_,
            BitBoard hash_,
            BitBoard castle_,
            PieceClass captured_,
            const ndarray<Square, COLOR_NB - 1>& enpass_
        ) 
        noexcept : 
          clock(clock_),
          turn(turn_),
          hash(hash_),
          castle(castle_),
          captured(captured_),
          enpass(enpass_) {}
};

class Position
{
    public:

        Board board;
        GameSetup setup;
        std::vector<GameState> states;

        Position()  noexcept = default;
        ~Position() noexcept = default;

        void makemove(Move move);
        void undomove(Move move);
};

} // namespace athena

#endif // #ifndef POSITION_H