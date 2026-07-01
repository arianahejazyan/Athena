#pragma once

#include "bitboard.h"
#include "chess/constants.h"
#include "square.h"
#include "color.h"
#include "piece.h"
#include <cstdint>

namespace athena::chess {

// [main, anti] diagonal bishop attacks
alignas(64) extern const 
std::array<std::pair<Bitboard, Bitboard>, VALID_NB> PRECOMPUTED_TABLE_DIAGONAL;
 
// [vertical, horizontal] rook attacks
alignas(64) extern const 
std::array<std::pair<Bitboard, Bitboard>, VALID_NB> PRECOMPUTED_TABLE_STRAIGHT;

// [kingt, knigh] attacks
alignas(64) extern const 
std::array<std::pair<Bitboard, Bitboard>, VALID_NB> PRECOMPUTED_TABLE_CRAWL;

// [red, yellow, blue, green] pawn attacks
alignas(64) extern const 
std::array<std::array<Bitboard, COLOR_NB>, VALID_NB> PRECOMPUTED_TABLE_PAWN;

template<Piece::ID piece>
inline const 
Bitboard get_slide_attacks(Square sq) noexcept {
    if constexpr (piece == Piece::ID::Bishop) { const auto& [main, anti] = PRECOMPUTED_TABLE_DIAGONAL[sq.compact()]; return main | anti; }
    if constexpr (piece == Piece::ID::Rook  ) { const auto& [vert, hori] = PRECOMPUTED_TABLE_STRAIGHT[sq.compact()]; return vert | hori; }
    return Bitboard();
}

template<Piece::ID piece>
inline const 
Bitboard get_crawl_attacks(Square sq) noexcept {
    if constexpr (piece == Piece::ID::King  ) { const auto& [attacks, _] = PRECOMPUTED_TABLE_CRAWL[sq.compact()]; return attacks; }
    if constexpr (piece == Piece::ID::Knight) { const auto& [_, attacks] = PRECOMPUTED_TABLE_CRAWL[sq.compact()]; return attacks; }
    return Bitboard();
}

inline const 
Bitboard get_pawn_attacks(Square sq, Color::ID color) noexcept {
    auto reorder = 
        ((static_cast<uint8_t>(color) & 0b10) >> 1) | 
        ((static_cast<uint8_t>(color) & 0b01) << 1);
    return PRECOMPUTED_TABLE_PAWN[sq.compact()][reorder];
}

template<Piece::ID piece>
inline const 
Bitboard get_slide_attacks(
    Square sq, const Bitboard& __restrict__ occupied) noexcept;

template<>
inline const 
Bitboard get_slide_attacks<Piece::ID::Bishop>(
    Square sq, const Bitboard& __restrict__ occupied) noexcept {
    const auto& [diag, anti] = PRECOMPUTED_TABLE_DIAGONAL[sq.compact()];

    auto d1 = diag & occupied;
    auto a1 = anti & occupied;

    auto d2 = d1; 
    auto a2 = a1;

    d2.swap_bytes();
    a2.swap_bytes();

    const Square sq_flip = sq.flip_rank();

    d1 -= sq;
    a1 -= sq;
    d2 -= sq_flip;
    a2 -= sq_flip;

    d2.swap_bytes();
    a2.swap_bytes();

    return ((d1 ^ d2) & diag) | ((a1 ^ a2) & anti);
}

template<>
inline const 
Bitboard get_slide_attacks<Piece::ID::Rook>(
    Square sq, const Bitboard& __restrict__ occupied) noexcept {
    const auto& [vert, hori] = PRECOMPUTED_TABLE_STRAIGHT[sq.compact()];

    auto v1 = vert & occupied;
    auto v2 = v1;
    v2.swap_bytes();

    const Square sq_flip = sq.flip_rank();

    v1 -= sq;
    v2 -= sq_flip;
    v2.swap_bytes();
    v1 &= vert;
    v2 &= vert;

    Bitboard h{0ULL};
    const auto index = 1ULL << sq.index();
    const auto chunk = sq.chunk();
    auto& c = h.chunk(chunk);
    const auto lower = occupied.chunk(chunk) & (index - 1);
    const auto upper = occupied.chunk(chunk) & ~lower;
    const auto lsb = 1  + __builtin_ctzll(upper | (1ULL << 63));
    const auto msb = 63 - __builtin_clzll(lower | (1ULL << 0 ));
    c = (1ULL << lsb) - (1ULL << msb) - index;

    return ((v1 ^ v2) & vert) | (h & hori);
}

alignas(64)
extern const 
std::array<std::array<std::pair<Bitboard, Bitboard>, VALID_NB>, VALID_NB> PRECOMPUTED_TABLE_THROUGH_BETWEEN;

inline const Bitboard get_line_through_mask(Square x, Square y) noexcept { return PRECOMPUTED_TABLE_THROUGH_BETWEEN[x.compact()][y.compact()].first ; }
inline const Bitboard get_line_between_mask(Square x, Square y) noexcept { return PRECOMPUTED_TABLE_THROUGH_BETWEEN[x.compact()][y.compact()].second; }

} // namespace athena