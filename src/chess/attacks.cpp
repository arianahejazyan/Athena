#include "attacks.h"
#include "offset.h"

namespace athena
{

// compute crawl attack mask

constexpr std::array<Straight, SQUARE_NB> STRAIGHT = []() 
{
    std::array<Straight, SQUARE_NB> arr {};

    for (int s = 0; s < SQUARE_NB; s++)
    {
        Square sq = Square(s);
        auto r = sq.rank();
        auto f = sq.file();

        if (sq.stone()) continue;

        auto offsets = slide_offsets(Piece::Rook);

        for (auto offset : {offsets[0], offsets[3]})
        {
            Square target = sq + offset;

            while (!target.stone())
            {
                arr[s].vertical.set(target);
                target += offset;
            }
        }

        for (auto offset : {offsets[1], offsets[2]})
        {
            Square target = sq + offset;

            while (!target.stone())
            {
                arr[s].horizontal.set(target);
                target += offset;
            }
        }
    }

    return arr;
}();


    // consteval auto fillv2()
    // {
    //     constexpr std::array<std::array<int, 2>, 2> diag_deltas = {{{+1, +1}, {-1, -1}}};
    //     constexpr std::array<std::array<int, 2>, 2> anti_deltas = {{{+1, -1}, {-1, +1}}};
    
    //     std::array<Diagonal, BOARDSIZE> arr{};
    
    //     int idx = 0;
    //     for (int sq = 0; sq < SQUARE_NB; sq++)
    //     {
    //         auto s = SQ(sq);
    //         auto r = s.rank();
    //         auto f = s.file();
    
    //         if (isInValidSquare(r, f))
    //             continue;
    
    //         for (auto [dr, df] : diag_deltas)
    //         {
    //             int tr = r + dr;
    //             int tf = f + df;
                
    //             while (!isInValidSquare(tr, tf))
    //             {
    //                 arr[idx].diag.toggleSQ(SQ(tr, tf));
    //                 tr += dr;
    //                 tf += df;
    //             }
    //         }
    
    //         for (auto [dr, df] : anti_deltas)
    //         {
    //             int tr = r + dr;
    //             int tf = f + df;
    
    //             while (!isInValidSquare(tr, tf))
    //             {
    //                 arr[idx].anti.toggleSQ(SQ(tr, tf));
    //                 tr += dr;
    //                 tf += df;
    //             }
    //         }
    
    //         idx++;
    //     }
    
    //     return arr;
    // }



constexpr std::array<Diagonal, SQUARE_NB> DIAGONAL = []() 
{
    std::array<Diagonal, SQUARE_NB> arr {};

    for (int s = 0; s < SQUARE_NB; s++)
    {
        Square sq = Square(s);
        auto r = sq.rank();
        auto f = sq.file();

        if (sq.stone()) continue;

        auto offsets = slide_offsets(Piece::Bishop);

        for (auto offset : {offsets[0], offsets[3]})
        {
            Square target = sq + offset;

            while (!target.stone())
            {
                arr[s].diag.set(target);
                target += offset;
            }
        }

        for (auto offset : {offsets[1], offsets[2]})
        {
            Square target = sq + offset;

            while (!target.stone())
            {
                arr[s].anti.set(target);
                target += offset;
            }
        }
    }

    return arr;
}();

constexpr std::array<Adjacent, SQUARE_NB> ADJACENT = []() 
{
    std::array<Adjacent, SQUARE_NB> arr {};

    for (int s = 0; s < SQUARE_NB; s++)
    {
        Square sq = Square(s);
        auto r = sq.rank();
        auto f = sq.file();

        if (sq.stone()) continue;

        for (auto offset : crawl_offsets(Piece::Knight))
        {
            Square target = sq + offset;

            if (target.stone()) continue;

            arr[s].knight.set(target);
        }

        for (auto offset : crawl_offsets(Piece::King))
        {
            Square target = sq + offset;

            if (target.stone()) continue;

            arr[s].king.set(target);
        }
    }

    return arr;
}();


consteval uint64_t pdep(uint64_t src, uint64_t mask)
{
    uint64_t result = 0;
    uint64_t bit = 1;

    while (mask)
    {
        uint64_t lsb = mask & -mask;
        mask -= lsb;

        if (src & bit)
            result |= lsb;

        bit <<= 1;
    }
    return result;
}

consteval std::array<std::array<uint64_t, 1024>, SQUARE_NB> fill_diag_pext_table()
{
    std::array<std::array<uint64_t, 1024>, SQUARE_NB> table {};

    Offset 
    offset1 = slide_offsets(Piece::Bishop)[3],
    offset2 = slide_offsets(Piece::Bishop)[0];

    for (Square sq = Square::E2; sq <= Square::L15; sq++)
    {
        if (sq.stone()) continue;

        for (uint64_t occupied = 0; occupied < 1024; occupied++)
        {
            uint64_t mask = 0;
  

            Bitboard diag(DIAGONAL[static_cast<uint8_t>(sq)].diag);
            diag &= pdep(occupied, diag.combine());

            for (Square bit = sq + offset1; !bit.stone(); bit += offset1)
            {
                mask |= 1ULL << bit.index();
                if (diag.has(bit)) break;
                // if (occupied & (1ULL << file(bit))) break;
            }

            for (Square bit = sq + offset2; !bit.stone(); bit += offset2)
            {
                mask |= 1ULL << bit.index();
                if (diag.has(bit)) break;
                // if (occupied & (1ULL << file(bit))) break;
            }

            table[static_cast<uint8_t>(sq)][occupied] = mask;
        }
    }

    return table;
}

consteval std::array<std::array<uint64_t, 1024>, SQUARE_NB> fill_anti_pext_table()
{
    std::array<std::array<uint64_t, 1024>, SQUARE_NB> table {};

    Offset 
    offset1 = slide_offsets(Piece::Bishop)[1],
    offset2 = slide_offsets(Piece::Bishop)[2];

    for (Square sq = Square::E2; sq <= Square::L15; sq++)
    {
        if (sq.stone()) continue;

        for (uint64_t occupied = 0; occupied < 1024; occupied++)
        {
            uint64_t mask = 0;
  

            Bitboard anti(DIAGONAL[static_cast<uint8_t>(sq)].anti);
            anti &= pdep(occupied, anti.combine());

            for (Square bit = sq + offset1; !bit.stone(); bit += offset1)
            {
                mask |= 1ULL << bit.index();
                if (anti.has(bit)) break;
                // if (occupied & (1ULL << file(bit))) break;
            }

            for (Square bit = sq + offset2; !bit.stone(); bit += offset2)
            {
                mask |= 1ULL << bit.index();
                if (anti.has(bit)) break;
                // if (occupied & (1ULL << file(bit))) break;
            }

            table[static_cast<uint8_t>(sq)][occupied] = mask;
        } 
    }

    return table;
}

constexpr std::array<std::array<uint64_t, 1024>, SQUARE_NB> PEXT_TABLE_DIAG = fill_diag_pext_table();
constexpr std::array<std::array<uint64_t, 1024>, SQUARE_NB> PEXT_TABLE_ANTI = fill_anti_pext_table();

consteval auto fill_pext_table_vertical() // key
{
    std::array<std::array<Bitboard, 4096>, RANK_NB> table {};

    auto offset = slide_offsets(Piece::Rook);

    for (int r = 1; r <= 14; r++)
    {
        for (uint64_t occupied = 0; occupied < 4096; occupied++)
        {
            Bitboard mask(0,0,0,0);

            int x = r;
            while (true)
            {
                x--;
                if (x == 0) break;
                mask.set(Square(x, 0));
                if (x == 1) break;
                if ((occupied << 2) & (1ULL << x)) break;
            }

            int y = r;
            while (true)
            {
                y++;
                if (y == 15) break;
                mask.set(Square(y, 0));
                if (y == 14) break;
                if ((occupied << 2) & (1ULL << y)) break;
            }

            table[r][occupied] = mask;
        }
    }

    return table;
}

constexpr std::array<std::array<Bitboard, 4096>, RANK_NB   > PEXT_TABLE_VERTICAL   = fill_pext_table_vertical();
// constexpr std::array<std::array<uint64_t, 4096>, CHUNK_SIZE> PEXT_TABLE_HORIZONTAL;

// constexpr std::array<Bitboard, FILE_NB - 2> VERTICAL_MASK_TABLE = []()
// {
//     std::array<Bitboard, FILE_NB - 2> table {};
//     for (int file =0; file < FILE_NB; file++)
//     {
//         if (file == 0 || file == 15) continue;
//         if (file <= 3 || file >= 12) table[file - 1] = Bitboard(0ULL      , UINT64_MAX, UINT64_MAX, 0ULL      );
//         if (file >= 4 && file <= 11) table[file - 1] = Bitboard(UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX);
//     }
//     return table;
// }();

// // constexpr std::array<Bitboard, FILE_NB - 2> VERTICAL_MASK_TABLE = []() consteval
// // {
// //     std::array<Bitboard, FILE_NB - 2> table {};
// //     for (auto file: file_array)
// //     {
// //         if (file == 0 || file == 15) continue;
// //         if (file <= 3 || file >= 12) table[(file - 1).value_] = Bitboard(0ULL      , UINT64_MAX, UINT64_MAX, 0ULL      );
// //         if (file >= 4 && file <= 11) table[(file - 1).value_] = Bitboard(UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX);
// //     }
// //     return table;
// // }();

// constexpr std::array<std::pair<Bitboard, Bitboard>, SQUARE_NB - 40> STONE_MASK_TABLE = []()
// {
//     std::array<std::pair<Bitboard, Bitboard>, SQUARE_NB - 40> table {};
//     for (auto sq: square_array())
//     {
//         if (Square::E2 > sq || sq > Square::L15) continue;
//         const auto rank = sq.rank();
//         const auto file = sq.file();
//         if (file == 0 || file == 15) continue;
//         if (rank == 0 || rank == 15) continue;
//         if (file <= 3 || file >= 12) table[(sq - 20).value_].first  = Bitboard(0ULL      , UINT64_MAX, UINT64_MAX, 0ULL      );
//         if (file >= 4 && file <= 11) table[(sq - 20).value_].first  = Bitboard(UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX);
//         if (rank <= 3 || rank >= 12) table[(sq - 20).value_].second = Bitboard(0ULL      , UINT64_MAX, UINT64_MAX, 0ULL      );
//         if (rank >= 4 && rank <= 11) table[(sq - 20).value_].second = Bitboard(UINT64_MAX, UINT64_MAX, UINT64_MAX, UINT64_MAX);
//     }
//     return table;
// }();

consteval Bitboard compute_pawn_attack_mask(Square source, Color color)
{
    Bitboard mask(0);
    for (auto offset: take_offsets(color))
    {
        Square target = source + offset;
        if (target.valid()) 
        {
            mask.set(target);
        } 
    }
    return mask;
}

constexpr std::array<std::array<std::pair<Bitboard, Bitboard>, TEAM_NB>, SQUARE_NB> PRECOMPUTED_PAWN_ATTACKS = []() consteval
{
    std::array<std::array<std::pair<Bitboard, Bitboard>, TEAM_NB>, SQUARE_NB> table;

    for (auto sq: squares_array())
    {
        if (sq.stone()) continue;

        for (auto team: team_array())
        {
            auto& pair = table[static_cast<uint8_t>(sq)][static_cast<uint8_t>(team)];
            pair.first  = compute_pawn_attack_mask(sq, team == Team::RY ? Color::Red    : Color::Blue );
            pair.second = compute_pawn_attack_mask(sq, team == Team::RY ? Color::Yellow : Color::Green);
        }
    }

    return table;
}();

alignas(64) 
constexpr std::array<std::array<Bitboard, COLOR_NB>, SQUARE_NB> PRECOMPUTED_PAWN_ATTACKS_2 = []() consteval {
    std::array<std::array<Bitboard, COLOR_NB>, SQUARE_NB> table{};

    for (auto sq: squares_array())
    {
        if (sq.stone()) continue;

        table[sq.value_][Color::Red   ] = compute_pawn_attack_mask(sq, Color::Red   );
        table[sq.value_][Color::Blue  ] = compute_pawn_attack_mask(sq, Color::Yellow);
        table[sq.value_][Color::Yellow] = compute_pawn_attack_mask(sq, Color::Blue  );
        table[sq.value_][Color::Green ] = compute_pawn_attack_mask(sq, Color::Green );
    }

    return table;
}();

} // namespace athena