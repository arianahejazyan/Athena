#pragma once

#include "chess.h"

namespace athena
{

struct GameState
{
    PieceClass capture;
    Square enpass;
    Castle castle;
    uint8_t fiftymove;
};

class alignas(CACHELINE_SIZE) Position
{
    public:
    Position() = default;

    void init(FEN fen);

    PieceClass board(Square sq) const noexcept {
        return board_[static_cast<uint8_t>(sq)];
    }

    GameState& state() noexcept {
        return states_[play_];
    }

    const GameState& state() const noexcept {
        return states_[play_];
    }

    Square royal(Color color) const noexcept {
        return royals_[static_cast<uint8_t>(color)];
    }

    Square enpass(Color color) const noexcept {
        return enpass_[static_cast<uint8_t>(color)];
    }

    Color turn() const noexcept {
        return turn_;
    }

    void makemove(Move move);
    void undomove(Move move);

    bool inCheck(Color color) const;
    bool inCheck(Color color, Square sq) const;

    void print() const;

    std::string fen() const;

    private:

    std::array<PieceClass, SQUARE_NB> board_;
    std::array<GameState, PLAY_NB> states_;
    std::array<Square, COLOR_NB> royals_;
    std::array<Square, COLOR_NB> enpass_;
    uint8_t play_;
    Color turn_;
};

} // namespace athena