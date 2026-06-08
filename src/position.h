#pragma once

#include "chess.h"
#include "bitboard.h"
#include "constants.h"
#include "square.h"
#include <cstdint>

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
    // Position(const GameSetup& setup): setup_(setup) {} // fix

    void init(FEN fen) noexcept;

    PieceClass board(Square sq) const noexcept {
        return board_[static_cast<uint8_t>(sq)];
    }

    GameState& state() noexcept {
        return states_[play_];
    }

    const GameState& state() const noexcept {
        return states_[play_];
    }

    Square royal() const noexcept {
        return royals_[static_cast<uint8_t>(turn_)];
    }

    Square royal(Color color) const noexcept {
        return royals_[static_cast<uint8_t>(color)];
    }

    // template<Color color>
    // Square royal() const noexcept {
    //     return royals_[static_cast<uint8_t>(color)];
    // }

    Square enpass(Color color) const noexcept {
        return enpass_[static_cast<uint8_t>(color)];
    }

    Color turn() const noexcept {
        return turn_;
    }

    Bitboard pieces(Piece piece) noexcept {
        return pieces_[static_cast<uint8_t>(piece)];
    }

    Bitboard colors(Color color) noexcept {
        return pieces_[static_cast<uint8_t>(color)];
    }

    Bitboard bitboard(Piece piece) const noexcept {
        return pieces_[static_cast<uint8_t>(piece)];
    }

    Bitboard bitboard(Color color) const noexcept {
        return colors_[static_cast<uint8_t>(color)];
    }

    Bitboard bitboard(Team team) const noexcept {
        return teams_[static_cast<uint8_t>(team)];
    }

    void setPiece(Square sq, PieceClass pc) noexcept
    {
        board_[static_cast<uint8_t>(sq)] = pc;

        pieces_[static_cast<uint8_t>(pc.piece())].set(sq);
        colors_[static_cast<uint8_t>(pc.color())].set(sq);

        teams_[static_cast<uint8_t>(pc.color().team())].set(sq);
    }

    void popPiece(Square sq) noexcept
    {
        Team team = board_[static_cast<uint8_t>(sq)].color().team();

        pieces_[static_cast<uint8_t>(board_[static_cast<uint8_t>(sq)].piece())].pop(sq);
        colors_[static_cast<uint8_t>(board_[static_cast<uint8_t>(sq)].color())].pop(sq);

        teams_[static_cast<uint8_t>(team)].pop(sq);

        board_[static_cast<uint8_t>(sq)] = PieceClass::Empty();
    }

    void makemove(Move move, const GameSetup setup);
    void undomove(Move move, const GameSetup setup);

    bool inCheck() const noexcept;
    bool inCheck(Color color) const noexcept;
    bool inCheck(Color color, Square sq) const noexcept;

    bool isLegal(Move move) const noexcept;

    void print() const; // board16x16 = false

    std::string fen() const;

    private:

    // compute check and pinned masks
    public:

    // const GameSetup& setup_;

    template<Color color>
    void compute_check_and_pinned_masks();

    std::array<PieceClass, SQUARE_NB> board_;
    std::array<GameState, PLAY_NB> states_;
    std::array<Bitboard, 6> pieces_;
    std::array<Bitboard, 4> colors_;
    std::array<Bitboard, 2> teams_;
    
    std::array<Square, COLOR_NB> royals_;
    std::array<Square, COLOR_NB> enpass_;
    uint8_t play_;
    Color turn_;

    // store check and pinned masks respectively
    // std::pair<Bitboard, Bitboard> check_pinned_masks_; // array
    // Bitboard checksmask_;
    // Bitboard pinnedmask_;

    std::array<std::pair<Bitboard, Bitboard>, PLAY_NB> check_pinned_masks_; // array

    public:

    void movegen() noexcept
    {
        const auto color = turn();
        switch (color.value_) 
        {
            case Color::Red   : compute_check_and_pinned_masks<Color::Red   >(); break;
            case Color::Blue  : compute_check_and_pinned_masks<Color::Blue  >(); break;
            case Color::Yellow: compute_check_and_pinned_masks<Color::Yellow>(); break;
            case Color::Green : compute_check_and_pinned_masks<Color::Green >(); break;
            default: break;
        }
    }

    // template<std::size_t chunk, uint8_t piece>
    // friend void process_chunk_candidates(Position& pos, Color turn, Square royal_square, Bitboard& candidates, Bitboard& checkers) noexcept;
};

} // namespace athena