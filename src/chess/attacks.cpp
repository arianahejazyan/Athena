#include "attacks.h"
#include "bitboard.h"
#include "constants.h"
#include "square.h"

namespace athena::chess {

consteval auto bitboard(
    Square sq, Square::Offset offset, bool slide) {
    Bitboard bb{0ULL};
    sq += offset;
    while (sq.isValid()) {
        bb.set_bit(sq);
        if (!slide) break;
        sq += offset;
    }
    return bb;
}

alignas(64) 
constexpr std::array<std::pair<Bitboard, Bitboard>, VALID_NB> 
PRECOMPUTED_TABLE_DIAGONAL = []() consteval {
    std::array<std::pair<Bitboard, Bitboard>, VALID_NB> table{};
    const auto offsets = Square::offsets<Piece::ID::Bishop>();
    for (int s = 0; s < SQUARE_NB; s++) {
        Square sq(static_cast<Square::ID>(s));
        if (sq.isStone()) continue;
        table[sq.compact()].first  = bitboard(sq, offsets[0], true) | bitboard(sq, offsets[3], true);
        table[sq.compact()].second = bitboard(sq, offsets[1], true) | bitboard(sq, offsets[2], true);
    }
    return table;
}();

alignas(64) 
constexpr std::array<std::pair<Bitboard, Bitboard>, VALID_NB> 
PRECOMPUTED_TABLE_STRAIGHT = []() consteval {
    std::array<std::pair<Bitboard, Bitboard>, VALID_NB> table{};
    const auto offsets = Square::offsets<Piece::ID::Rook>();
    for (int s = 0; s < SQUARE_NB; s++) {
        Square sq(static_cast<Square::ID>(s));
        if (sq.isStone()) continue;
        table[sq.compact()].first  = bitboard(sq, offsets[0], true) | bitboard(sq, offsets[3], true);
        table[sq.compact()].second = bitboard(sq, offsets[1], true) | bitboard(sq, offsets[2], true);
    }
    return table;
}();

alignas(64) 
constexpr std::array<std::pair<Bitboard, Bitboard>, VALID_NB>
PRECOMPUTED_TABLE_CRAWL = []() consteval {
    std::array<std::pair<Bitboard, Bitboard>, VALID_NB> table{};
    const auto offsets_1 = Square::offsets<Piece::ID::King  >();
    const auto offsets_2 = Square::offsets<Piece::ID::Knight>();
    for (int s = 0; s < SQUARE_NB; s++) {
        Square sq(static_cast<Square::ID>(s));
        if (sq.isStone()) continue;
        for (int i=0; i<CRAWL_NB; i++) table[sq.compact()].first  |= bitboard(sq, offsets_1[i], false);
        for (int i=0; i<CRAWL_NB; i++) table[sq.compact()].second |= bitboard(sq, offsets_2[i], false);
    }
    return table;
}();


alignas(64) 
constexpr std::array<std::array<Bitboard, COLOR_NB>, VALID_NB> 
PRECOMPUTED_TABLE_PAWN = []() consteval {
    std::array<std::array<Bitboard, COLOR_NB>, VALID_NB>  table{};
    for (int s = 0; s < SQUARE_NB; s++) {
        Square sq(static_cast<Square::ID>(s));
        if (sq.isStone()) continue;
        for (auto color: {
            Color::ID::Red,
            Color::ID::Blue,
            Color::ID::Yellow,
            Color::ID::Green
        }) {
            table[sq.compact()][static_cast<uint8_t>(color)] |= 
                bitboard(sq, Square::take(color, 0), false) | 
                bitboard(sq, Square::take(color, 1), false);
        }
    }
    return table;
}();

// alignas(64) 
// constexpr std::array<std::pair<Bitboard, Bitboard>, VALID_NB> 
// PRECOMPUTED_TABLE_RY = []() consteval {
//     std::array<std::pair<Bitboard, Bitboard>, VALID_NB> table{};
//     const auto offsets_r = Square::offsets<Color::ID::Red   >();
//     const auto offsets_y = Square::offsets<Color::ID::Yellow>();
//     for (int s = 0; s < SQUARE_NB; s++) {
//         Square sq(static_cast<Square::ID>(s));
//         if (sq.isStone()) continue;
//         table[sq.compact()].first  |= bitboard(sq, offsets_r[0], false) | bitboard(sq, offsets_r[1], false);
//         table[sq.compact()].second |= bitboard(sq, offsets_y[0], false) | bitboard(sq, offsets_y[1], false);
//     }
//     return table;
// }();

// alignas(64) 
// constexpr std::array<std::pair<Bitboard, Bitboard>, VALID_NB>
// PRECOMPUTED_TABLE_BG = []() consteval {
//     std::array<std::pair<Bitboard, Bitboard>, VALID_NB> table{};
//     const auto offsets_b = Square::offsets<Color::ID::Blue >();
//     const auto offsets_g = Square::offsets<Color::ID::Green>();
//     for (int s = 0; s < SQUARE_NB; s++) {
//         Square sq(static_cast<Square::ID>(s));
//         if (sq.isStone()) continue;
//         table[sq.compact()].first  |= bitboard(sq, offsets_b[0], false) | bitboard(sq, offsets_b[1], false);
//         table[sq.compact()].second |= bitboard(sq, offsets_g[0], false) | bitboard(sq, offsets_g[1], false);
//     }
//     return table;
// }();

alignas(64)
constexpr std::array<std::array<std::pair<Bitboard, Bitboard>, VALID_NB>, VALID_NB>
PRECOMPUTED_TABLE_THROUGH_BETWEEN = []() consteval {
    std::array<std::array<std::pair<Bitboard, Bitboard>, VALID_NB>, VALID_NB> table{};
    for (int s1 = 0; s1 < SQUARE_NB; s1++) {
    Square sq1(static_cast<Square::ID>(s1));
    if (sq1.isStone()) continue;
    for (int s2 = 0; s2 < SQUARE_NB; s2++) {
    Square sq2(static_cast<Square::ID>(s2));
    if (sq2.isStone()) continue;
    if (s1 == s2 )     continue;

        const auto r1 = sq1.rank(), r2 = sq2.rank();
        const auto f1 = sq1.file(), f2 = sq2.file();

        const int dr = r2 - r1;
        const int df = f2 - f1;

        const bool same_rank = (r1 == r2);
        const bool same_file = (f1 == f2);
        const bool same_diag = (dr == df);
        const bool same_anti = (dr == -df);

        if (!same_rank && 
            !same_file && 
            !same_diag && 
            !same_anti) continue;

        Bitboard through{};
        Bitboard between{};

        if (same_file) through = PRECOMPUTED_TABLE_STRAIGHT[sq1.compact()].first  | PRECOMPUTED_TABLE_STRAIGHT[sq2.compact()].first ; 
        if (same_rank) through = PRECOMPUTED_TABLE_STRAIGHT[sq1.compact()].second | PRECOMPUTED_TABLE_STRAIGHT[sq2.compact()].second; 
        if (same_diag) through = PRECOMPUTED_TABLE_DIAGONAL[sq1.compact()].first  | PRECOMPUTED_TABLE_DIAGONAL[sq2.compact()].first ; 
        if (same_anti) through = PRECOMPUTED_TABLE_DIAGONAL[sq1.compact()].second | PRECOMPUTED_TABLE_DIAGONAL[sq2.compact()].second; 

        auto r = Square::offsets<Piece::ID::Rook  >();
        auto b = Square::offsets<Piece::ID::Bishop>();

        Square::Offset offset =
        same_rank ? (df > 0 ? r[2] : r[1]):
        same_file ? (dr > 0 ? r[3] : r[0]):
        same_diag ? (dr > 0 ? b[3] : b[0]):
                    (dr > 0 ? b[2] : b[1]);

        Square curr = sq1;
        curr += offset;
        while (curr != sq2 && curr.isValid()) {
            between.set_bit(curr);
            curr += offset;
        }

        table[sq1.compact()][sq2.compact()] = { through, between };
    }}
    return table;
}();

} // namespace athena