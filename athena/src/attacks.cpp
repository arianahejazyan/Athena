#include "attacks.h"
#include "chess.h"
#include <cstdint>

namespace athena
{


constexpr std::array<Straight, SQUARE_NB> STRAIGHT = []() 
{
    std::array<Straight, SQUARE_NB> arr {};

    for (int s = 0; s < SQUARE_NB; s++)
    {
        Square sq = Square(s);
        auto r = rank(sq);
        auto f = file(sq);

        if (isStone(sq)) continue;

        auto offsets = slide_offsets(Piece::Rook);

        for (auto offset : {offsets[0], offsets[3]})
        {
            Square target = sq + offset;

            while (!isStone(target))
            {
                arr[s].vertical.setSquare(target);
                target += offset;
            }
        }

        for (auto offset : {offsets[1], offsets[2]})
        {
            Square target = sq + offset;

            while (!isStone(target))
            {
                arr[s].horizontal.setSquare(target);
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
        auto r = rank(sq);
        auto f = file(sq);

        if (isStone(sq)) continue;

        auto offsets = slide_offsets(Piece::Bishop);

        for (auto offset : {offsets[0], offsets[3]})
        {
            Square target = sq + offset;

            while (!isStone(target))
            {
                arr[s].diag.setSquare(target);
                target += offset;
            }
        }

        for (auto offset : {offsets[1], offsets[2]})
        {
            Square target = sq + offset;

            while (!isStone(target))
            {
                arr[s].anti.setSquare(target);
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
        auto r = rank(sq);
        auto f = file(sq);

        if (isStone(sq)) continue;

        for (auto offset : crawl_offsets(Piece::Knight))
        {
            Square target = sq + offset;

            if (isStone(target)) continue;

            arr[s].knight.setSquare(target);
        }

        for (auto offset : crawl_offsets(Piece::King))
        {
            Square target = sq + offset;

            if (isStone(target)) continue;

            arr[s].king.setSquare(target);
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
        if (isStone(sq)) continue;

        for (uint64_t occupied = 0; occupied < 1024; occupied++)
        {
            uint64_t mask = 0;
  

            Bitboard diag(DIAGONAL[static_cast<uint8_t>(sq)].diag);
            diag &= pdep(occupied, diag.combine());

            for (Square bit = sq + offset1; !isStone(bit); bit += offset1)
            {
                mask |= 1ULL << index(bit);
                if (diag.hasSquare(bit)) break;
                // if (occupied & (1ULL << file(bit))) break;
            }

            for (Square bit = sq + offset2; !isStone(bit); bit += offset2)
            {
                mask |= 1ULL << index(bit);
                if (diag.hasSquare(bit)) break;
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
        if (isStone(sq)) continue;

        for (uint64_t occupied = 0; occupied < 1024; occupied++)
        {
            uint64_t mask = 0;
  

            Bitboard anti(DIAGONAL[static_cast<uint8_t>(sq)].anti);
            anti &= pdep(occupied, anti.combine());

            for (Square bit = sq + offset1; !isStone(bit); bit += offset1)
            {
                mask |= 1ULL << index(bit);
                if (anti.hasSquare(bit)) break;
                // if (occupied & (1ULL << file(bit))) break;
            }

            for (Square bit = sq + offset2; !isStone(bit); bit += offset2)
            {
                mask |= 1ULL << index(bit);
                if (anti.hasSquare(bit)) break;
                // if (occupied & (1ULL << file(bit))) break;
            }

            table[static_cast<uint8_t>(sq)][occupied] = mask;
        } 
    }

    return table;
}

constexpr std::array<std::array<uint64_t, 1024>, SQUARE_NB> PEXT_TABLE_DIAG = fill_diag_pext_table();
constexpr std::array<std::array<uint64_t, 1024>, SQUARE_NB> PEXT_TABLE_ANTI = fill_anti_pext_table();

consteval auto fill_pext_table_vertical()
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
                mask.setSquare(buildSquare(x, 0));
                if (x == 1) break;
                if (occupied & (1ULL << x)) break;
            }

            int y = r;
            while (true)
            {
                y++;
                if (y == 15) break;
                mask.setSquare(buildSquare(y, 0));
                if (y == 14) break;
                if (occupied & (1ULL << y)) break;
            }

            table[r][occupied] = mask;
        }
    }

    return table;
}

constexpr std::array<std::array<Bitboard, 4096>, RANK_NB   > PEXT_TABLE_VERTICAL   = fill_pext_table_vertical();
// constexpr std::array<std::array<uint64_t, 4096>, CHUNK_SIZE> PEXT_TABLE_HORIZONTAL;

} // namespace athena