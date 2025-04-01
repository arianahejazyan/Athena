#ifndef POSITION_H
#define POSITION_H

#include "board.h"
#include "chess.h"
#include "types.h"
#include "gamestate.h"
#include "move.h"

namespace Talia
{

struct Position
{
    private:

        StaticArray<GameState, 512> states;

    public:

        std::vector<Move> moveHistory;
        BitBoard pinned;
        BitBoard checks;
        BitBoard threat;
        Board board;
        Color turn;
        int size;

        Position(): size(0)
        {
        }

        ~Position() = default;

        [[nodiscard]] inline GameState& state()
        {
            return states[size];
        }

        [[nodiscard]] inline const GameState& state() const
        {
            return states[size];
        }

        [[nodiscard]] inline BitBoard teammate() const
        {
            return board.occ(turn);
        }

        [[nodiscard]] inline BitBoard opponent() const
        {
            return board.occ(next(turn));
        }

        [[nodiscard]] inline BitBoard everyone() const
        {
            return board.occ();
        }
        
        [[nodiscard]] inline BitBoard teammate(const Piece piece) const
        {
            return board.occ(piece, turn);
        }

        [[nodiscard]] inline BitBoard opponent(const Piece piece) const
        {
            return board.occ(piece, next(turn));
        }

        void computePinned();
        void computeChecks();
        void computeThreat();

        void makemove(const Move move);
        void undomove(const Move move);
};

} // namespace

#endif