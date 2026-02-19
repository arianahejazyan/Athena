#pragma once

#include "chess.h"
#include "bitboard.h"
#include "square.h"

namespace athena
{

inline constexpr int max_step(Piece piece) noexcept
{
    return piece == Piece::Rook ? 13 : 10;
}

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

    void setPiece(Square sq, PieceClass pc) noexcept
    {
        board_[static_cast<uint8_t>(sq)] = pc;

        pieces_[static_cast<uint8_t>(pc.piece())].set(sq);
        colors_[static_cast<uint8_t>(pc.color())].set(sq);
    }

    void popPiece(Square sq) noexcept
    {
        pieces_[static_cast<uint8_t>(board_[static_cast<uint8_t>(sq)].piece())].pop(sq);
        colors_[static_cast<uint8_t>(board_[static_cast<uint8_t>(sq)].color())].pop(sq);

        board_[static_cast<uint8_t>(sq)] = PieceClass::Empty();
    }

    void makemove(Move move);
    void undomove(Move move);

    bool inCheck(Color color) const;
    bool inCheck(Color color, Square sq) const;

    void print() const;

    std::string fen() const;

    private:

    // compute check and pinned masks
    void compute_masks();

    std::array<PieceClass, SQUARE_NB> board_;
    std::array<GameState, PLAY_NB> states_;
    std::array<Bitboard, 8> pieces_;
    std::array<Bitboard, 8> colors_;
    
    std::array<Square, COLOR_NB> royals_;
    std::array<Square, COLOR_NB> enpass_;
    uint8_t play_;
    Color turn_;

    // store check and pinned masks respectively
    std::pair<Bitboard, Bitboard> masks_;
};

} // namespace athena