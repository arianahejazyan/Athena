#include <initializer_list>
#include <iostream>
#include "bitboard.h"

namespace athena
{

// consteval BB buildBB(std::initializer_list<Square> squares)
// {
//     BB bb;
//     for (auto sq : squares) bb.toggleSQ(sq);
//     return bb;
// }

// constexpr std::array<RightsInfo, CASTLE_NB> RIGHTS_INFO =
// {{
//     {{ buildBB({J2 , K2 }) }, I2 , J2 , {} }, {{ buildBB({F2 , G2 , H2 }) }, I2 , H2 , {} },
//     {{ buildBB({B6 , B7 }) }, B8 , B7 , {} }, {{ buildBB({B9 , B10, B11}) }, B8 , B9 , {} },
//     {{ buildBB({F15, G15}) }, H15, G15, {} }, {{ buildBB({I15, J15, K15}) }, H15, I15, {} },
//     {{ buildBB({O10, O11}) }, O9 , O10, {} }, {{ buildBB({O6 , O7 , O8 }) }, O9 , O8 , {} },
//     {{ buildBB({J2 , K2 }) }, I2 , J2 , {} }, {{ buildBB({F2 , G2 , H2 }) }, I2 , H2 , {} },
//     {{ buildBB({B10, B11}) }, B9 , B10, {} }, {{ buildBB({B6 , B7 , B8 }) }, B9 , B8 , {} },
//     {{ buildBB({F15, G15}) }, H15, G15, {} }, {{ buildBB({I15, J15, K15}) }, H15, I15, {} },
//     {{ buildBB({O6 , O7 }) }, O8 , O7 , {} }, {{ buildBB({O9 , O10, O11}) }, O8 , O9 , {} },
// }};

// alignas(CACHELINE_SIZE)
// constexpr std::array<CastleInfo, CASTLE_NB> CASTLE_INFO =
// {{
//     { I2 , K2 , L2 , J2 },  { I2 , G2 , E2 , H2 }, 
//     { B8 , B6 , B5 , B7 },  { B8 , B10, B12, B9 },
//     { H15, F15, E15, G15},  { H15, J15, L15, I15}, 
//     { O9 , O11, O12, O10},  { O9 , O7 , O5 , O8 },
//     { I2 , K2 , L2 , J2 },  { I2 , G2 , E2 , H2 }, 
//     { B9 , B11, B12, B10},  { B9 , B7 , B5 , B8 },
//     { H15, F15, E15, G15},  { H15, J15, L15, I15}, 
//     { O8 , O6 , O5 , O7 },  { O8 , O10, O12, O9 },
// }};

// constexpr std::array<PawnInfo, (COLOR_NB - 4)> PAWN_INFO =
// {{
//     {buildBB({B11, C11, D11, E11, F11, G11, H11, I11, J11, K11, L11, M11, N11, O11}), buildBB({E3 , F3 , G3 , H3 , I3 , J3 , K3 , L3 , E4 , F4 , G4 , H4 , I4 , J4 , K4 , L4 })},
//     {buildBB({K2 , K3 , K4 , K5 , K6 , K7 , K8 , K9 , K10, K11, K12, K13, K14, K15}), buildBB({C5 , C6 , C7 , C8 , C9 , C10, C11, C12, D5 , D6 , D7 , D8 , D9 , D10, D11, D12})},
//     {buildBB({B6 , C6 , D6 , E6 , F6 , G6 , H6 , I6 , J6 , K6 , L6 , M6 , N6 , O6 }), buildBB({E14, F14, G14, H14, I14, J14, K14, L14, E13, F13, G13, H13, I13, J13, K13, L13})},
//     {buildBB({F2 , F3 , F4 , F5 , F6 , F7 , F8 , F9 , F10, F11, F12, F13, F14, F15}), buildBB({N5 , N6 , N7 , N8 , N9 , N10, N11, N12, M5 , M6 , M7 , M8 , M9 , M10, M11, M12})},
// }};


// constexpr std::array<RightsInfo, CASTLE_NB> RIGHTS_INFO =
// {{
//     {{ buildBB({SQ::J2 , SQ::K2 }) }, SQ::I2 , SQ::J2 , {} }, {{ buildBB({SQ::F2 , SQ::G2 , SQ::H2 }) }, SQ::I2 , SQ::H2 , {} },
//     {{ buildBB({SQ::B6 , SQ::B7 }) }, SQ::B8 , SQ::B7 , {} }, {{ buildBB({SQ::B9 , SQ::B10, SQ::B11}) }, SQ::B8 , SQ::B9 , {} },
//     {{ buildBB({SQ::F15, SQ::G15}) }, SQ::H15, SQ::G15, {} }, {{ buildBB({SQ::I15, SQ::J15, SQ::K15}) }, SQ::H15, SQ::I15, {} },
//     {{ buildBB({SQ::O10, SQ::O11}) }, SQ::O9 , SQ::O10, {} }, {{ buildBB({SQ::O6 , SQ::O7 , SQ::O8 }) }, SQ::O9 , SQ::O8 , {} },
//     {{ buildBB({SQ::J2 , SQ::K2 }) }, SQ::I2 , SQ::J2 , {} }, {{ buildBB({SQ::F2 , SQ::G2 , SQ::H2 }) }, SQ::I2 , SQ::H2 , {} },
//     {{ buildBB({SQ::B10, SQ::B11}) }, SQ::B9 , SQ::B10, {} }, {{ buildBB({SQ::B6 , SQ::B7 , SQ::B8 }) }, SQ::B9 , SQ::B8 , {} },
//     {{ buildBB({SQ::F15, SQ::G15}) }, SQ::H15, SQ::G15, {} }, {{ buildBB({SQ::I15, SQ::J15, SQ::K15}) }, SQ::H15, SQ::I15, {} },
//     {{ buildBB({SQ::O6 , SQ::O7 }) }, SQ::O8 , SQ::O7 , {} }, {{ buildBB({SQ::O9 , SQ::O10, SQ::O11}) }, SQ::O8 , SQ::O9 , {} },
// }};

// alignas(CACHELINE_SIZE)
// constexpr std::array<CastleInfo, CASTLE_NB> CASTLE_INFO =
// {{
//     { SQ::I2 , SQ::K2 , SQ::L2 , SQ::J2 },  { SQ::I2 , SQ::G2 , SQ::E2 , SQ::H2 }, 
//     { SQ::B8 , SQ::B6 , SQ::B5 , SQ::B7 },  { SQ::B8 , SQ::B10, SQ::B12, SQ::B9 },
//     { SQ::H15, SQ::F15, SQ::E15, SQ::G15},  { SQ::H15, SQ::J15, SQ::L15, SQ::I15}, 
//     { SQ::O9 , SQ::O11, SQ::O12, SQ::O10},  { SQ::O9 , SQ::O7 , SQ::O5 , SQ::O8 },
//     { SQ::I2 , SQ::K2 , SQ::L2 , SQ::J2 },  { SQ::I2 , SQ::G2 , SQ::E2 , SQ::H2 }, 
//     { SQ::B9 , SQ::B11, SQ::B12, SQ::B10},  { SQ::B9 , SQ::B7 , SQ::B5 , SQ::B8 },
//     { SQ::H15, SQ::F15, SQ::E15, SQ::G15},  { SQ::H15, SQ::J15, SQ::L15, SQ::I15}, 
//     { SQ::O8 , SQ::O6 , SQ::O5 , SQ::O7 },  { SQ::O8 , SQ::O10, SQ::O12, SQ::O9 },
// }};

// constexpr std::array<PawnInfo, (COLOR_NB - 4)> PAWN_INFO =
// {{
//     {buildBB({SQ::B11, SQ::C11, SQ::D11, SQ::E11, SQ::F11, SQ::G11, SQ::H11, SQ::I11, SQ::J11, SQ::K11, SQ::L11, SQ::M11, SQ::N11, SQ::O11}), buildBB({SQ::E3 , SQ::F3 , SQ::G3 , SQ::H3 , SQ::I3 , SQ::J3 , SQ::K3 , SQ::L3 , SQ::E4 , SQ::F4 , SQ::G4 , SQ::H4 , SQ::I4 , SQ::J4 , SQ::K4 , SQ::L4 })},
//     {buildBB({SQ::K2 , SQ::K3 , SQ::K4 , SQ::K5 , SQ::K6 , SQ::K7 , SQ::K8 , SQ::K9 , SQ::K10, SQ::K11, SQ::K12, SQ::K13, SQ::K14, SQ::K15}), buildBB({SQ::C5 , SQ::C6 , SQ::C7 , SQ::C8 , SQ::C9 , SQ::C10, SQ::C11, SQ::C12, SQ::D5 , SQ::D6 , SQ::D7 , SQ::D8 , SQ::D9 , SQ::D10, SQ::D11, SQ::D12})},
//     {buildBB({SQ::B6 , SQ::C6 , SQ::D6 , SQ::E6 , SQ::F6 , SQ::G6 , SQ::H6 , SQ::I6 , SQ::J6 , SQ::K6 , SQ::L6 , SQ::M6 , SQ::N6 , SQ::O6 }), buildBB({SQ::E14, SQ::F14, SQ::G14, SQ::H14, SQ::I14, SQ::J14, SQ::K14, SQ::L14, SQ::E13, SQ::F13, SQ::G13, SQ::H13, SQ::I13, SQ::J13, SQ::K13, SQ::L13})},
//     {buildBB({SQ::F2 , SQ::F3 , SQ::F4 , SQ::F5 , SQ::F6 , SQ::F7 , SQ::F8 , SQ::F9 , SQ::F10, SQ::F11, SQ::F12, SQ::F13, SQ::F14, SQ::F15}), buildBB({SQ::N5 , SQ::N6 , SQ::N7 , SQ::N8 , SQ::N9 , SQ::N10, SQ::N11, SQ::N12, SQ::M5 , SQ::M6 , SQ::M7 , SQ::M8 , SQ::M9 , SQ::M10, SQ::M11, SQ::M12})},
// }};


consteval auto fillv1()
{
    constexpr std::array<std::array<int, 2>, 2> deltas = {{{1, 0}, {-1, 0}}};
    
    std::array<Straight, BOARDSIZE> arr{};

    for (int sq = 0, idx = 0; sq < SQUARE_NB; sq++)
    {
        auto s = SQ(sq);
        auto r = s.rank();
        auto f = s.file();

        if (isInValidSquare(r, f))
            continue;

        for (auto [dr, df] : deltas)
        {
            int tr = r + dr;
            int tf = f + df;

            while (!isInValidSquare(tr, tf))
            {
                arr[idx].verti.toggleSQ(SQ(tr, tf));
                tr += dr;
                tf += df;
            }
        }

        auto chunk = s.chunk();
        auto index = s.index();
        auto layer = index >> 4;

        arr[idx].horiz = 0ULL;
        arr[idx].lower = 0ULL;
        arr[idx].upper = 0ULL;

        for (int col = 0; col < FILE_NB; col++)
        {
            if (isInValidSquare(r, col))
                continue;

            if (col < f) arr[idx].lower ^= 1ULL << ((layer << 4) | col);
            if (col > f) arr[idx].upper ^= 1ULL << ((layer << 4) | col);
        }

        arr[idx].horiz ^= arr[idx].lower;
        arr[idx].horiz ^= arr[idx].upper;

        idx++;
    }

    return arr;
}

consteval auto fillv2()
{
    constexpr std::array<std::array<int, 2>, 2> diag_deltas = {{{+1, +1}, {-1, -1}}};
    constexpr std::array<std::array<int, 2>, 2> anti_deltas = {{{+1, -1}, {-1, +1}}};

    std::array<Diagonal, BOARDSIZE> arr{};

    int idx = 0;
    for (int sq = 0; sq < SQUARE_NB; sq++)
    {
        auto s = SQ(sq);
        auto r = s.rank();
        auto f = s.file();

        if (isInValidSquare(r, f))
            continue;

        for (auto [dr, df] : diag_deltas)
        {
            int tr = r + dr;
            int tf = f + df;
            
            while (!isInValidSquare(tr, tf))
            {
                arr[idx].diag.toggleSQ(SQ(tr, tf));
                tr += dr;
                tf += df;
            }
        }

        for (auto [dr, df] : anti_deltas)
        {
            int tr = r + dr;
            int tf = f + df;

            while (!isInValidSquare(tr, tf))
            {
                arr[idx].anti.toggleSQ(SQ(tr, tf));
                tr += dr;
                tf += df;
            }
        }

        idx++;
    }

    return arr;
}

consteval auto fillv3()
{
    constexpr std::array<std::array<int, 2>, 8>   king_deltas = {{{+1,+1},{+1, 0},{+1,-1},{0 ,-1},{0 ,+1},{-1,+1},{-1, 0},{-1,-1}}};
    constexpr std::array<std::array<int, 2>, 8> knight_deltas = {{{+2,+1},{+2,-1},{+1,+2},{+1,-2},{-2,+1},{-2,-1},{-1,+2},{-1,-2}}};

    std::array<Adjacent, BOARDSIZE> arr{};

    int idx = 0;
    for (int sq = 0; sq < SQUARE_NB; sq++)
    {
        auto s = SQ(sq);
        auto r = s.rank();
        auto f = s.file();

        if (isInValidSquare(r, f))
            continue;

        for (auto [dr, df] : king_deltas)
        {
            int tr = r + dr;
            int tf = f + df;

            if (!isInValidSquare(tr, tf))
                arr[idx].king.toggleSQ(SQ(tr, tf));
        }

        for (auto [dr, df] : knight_deltas)
        {
            int tr = r + dr;
            int tf = f + df;

            if (!isInValidSquare(tr, tf))
                arr[idx].knight.toggleSQ(SQ(tr, tf));
        }

        idx++;
    }

    return arr;
}

alignas(CACHELINE_SIZE) constexpr std::array<Straight, BOARDSIZE> STRAIGHT = fillv1();
alignas(CACHELINE_SIZE) constexpr std::array<Diagonal, BOARDSIZE> DIAGONAL = fillv2();
alignas(CACHELINE_SIZE) constexpr std::array<Adjacent, BOARDSIZE> ADJACENT = fillv3();

void BitBoard::print(bool debug) const
{
    const int size   = debug ? 16 : 14;
    const int offset = debug ? 0  : 1 ;

    std::cout << "\n    ";
    for (int file = 0; file < size; ++file)
        std::cout << static_cast<char>('a' + file) << ' ';
    std::cout << "\n";

    for (int rank = size - 1; rank >= 0; --rank)
    {
        int row_label = rank + 1;
        std::cout << (row_label < 10 ? " " : "") << row_label << "  ";
        for (int file = 0; file < size; ++file)
        {
            int r = rank + offset;
            int f = file + offset;
            auto sq = SQ(r, f);
            bool valid = isInValidSquare(r, f);
            bool set = checkSQ(sq);
            if (set)
                 std::cout << (!valid ? "\033[34mX\033[0m" : "\033[90mX\033[0m");
            else std::cout << (!valid ? "." : "\033[90m.\033[0m");
            if (file < size - 1) std::cout << ' ';
        }
        std::cout << "  " << (row_label < 10 ? " " : "") << row_label << '\n';
    }

    std::cout << "    ";
    for (int file = 0; file < size; ++file)
        std::cout << static_cast<char>('a' + file) << ' ';
    std::cout << "\n" << std::endl;
}

} // namespace athena