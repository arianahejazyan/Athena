#ifndef BITBOARD_H
#define BITBOARD_H

#include <array>
#include "chess.h"

#include <iostream> // del

namespace athena
{

class BitBoard;

using BB        = BitBoard;
using u256      = BitBoard;
using uint256_t = BitBoard;

struct alignas(CACHELINE_SIZE) BitBoard
{
    u64 chunks[CHUNK_NB];

    constexpr BitBoard()
        : chunks{0, 0, 0, 0} {}

    // consteval BitBoard(std::initializer_list<Square> squares)
    //     : chunks{0, 0, 0, 0} { for (auto sq : squares) setSQ(sq); }

    constexpr BitBoard(u64 a, u64 b, u64 c, u64 d)
        : chunks{a, b, c, d} {}

    constexpr BitBoard(const BB& other)
    {
        chunks[0] = other.chunks[0];
        chunks[1] = other.chunks[1];
        chunks[2] = other.chunks[2];
        chunks[3] = other.chunks[3];
    }

    constexpr BitBoard(const BB& other1, const BB& other2)
    {
        chunks[0] = (other1.chunks[0] ^ other2.chunks[0]);
        chunks[1] = (other1.chunks[1] ^ other2.chunks[1]);
        chunks[2] = (other1.chunks[2] ^ other2.chunks[2]);
        chunks[3] = (other1.chunks[3] ^ other2.chunks[3]);
    }

    constexpr BB& operator=(const BB& other)
    {
        chunks[0] = other.chunks[0];
        chunks[1] = other.chunks[1];
        chunks[2] = other.chunks[2];
        chunks[3] = other.chunks[3];
        return *this;
    }


//     constexpr BitBoard() noexcept : chunks{0, 0, 0, 0} {}

//     constexpr BitBoard(Garbage) {}

//     constexpr BitBoard(const BitBoard& other)
//     {
//         chunks[0] = other.chunks[0];
//         chunks[1] = other.chunks[1];
//         chunks[2] = other.chunks[2];
//         chunks[3] = other.chunks[3];
//     }

//     constexpr BitBoard(uint64_t a, uint64_t b, uint64_t c, uint64_t d) noexcept
//         : chunks{a, b, c, d} {}

//     consteval BitBoard(std::initializer_list<Square> squares) noexcept
//         : chunks{0, 0, 0, 0}
//     {
//         for (auto sq : squares)
//             setSQ(sq);
//     }

//     inline void operator=(const BitBoard &other)
//     {
//         std::memcpy(this, &other, sizeof(BitBoard));
//     };

//     BitBoard &operator=(BitBoard &&) = default;

    constexpr inline BB operator~() const {
        return BB(~chunks[0], ~chunks[1], ~chunks[2], ~chunks[3]);
    }

    constexpr inline BB& operator&=(const BB& other)
    {
        chunks[0] &= other.chunks[0];
        chunks[1] &= other.chunks[1];
        chunks[2] &= other.chunks[2];
        chunks[3] &= other.chunks[3];
        return *this;
    }

//     constexpr inline BitBoard &operator|=(const BitBoard &other)
//     {
//         chunks[0] |= other.chunks[0];
//         chunks[1] |= other.chunks[1];
//         chunks[2] |= other.chunks[2];
//         chunks[3] |= other.chunks[3];
//         return *this;
//     }

    constexpr inline BB& operator^=(const BB& other)
    {
        chunks[0] ^= other.chunks[0];
        chunks[1] ^= other.chunks[1];
        chunks[2] ^= other.chunks[2];
        chunks[3] ^= other.chunks[3];
        return *this;
    }

//     constexpr inline bool operator==(const BitBoard &other) const
//     {
//         return (chunks[0] == other.chunks[0]) &&
//                (chunks[1] == other.chunks[1]) &&
//                (chunks[2] == other.chunks[2]) &&
//                (chunks[3] == other.chunks[3]);
//     }

//     constexpr inline bool operator!=(const BitBoard &other) const {
//         return !(*this == other);
//     }

    template<std::size_t chunk>
    inline constexpr void toggleSQ(u64 x) {
        chunks[chunk] ^= x;
    }


    inline constexpr void toggleSQ(std::size_t chunk, u64 index) {
        chunks[chunk] ^= index;
    }

    inline constexpr void toggleSQ(SQ sq) {
        toggleSQ(sq.chunk(), (1ULL << sq.index()));
    }

    // constexpr inline void setSQ(SQ sq) {
    //     chunks[chunkSQ(sq)] |= (1ULL << indexSQ(sq));
    // }

    // constexpr inline void popSQ(SQ sq) {
    //     chunks[chunkSQ(sq)] &= ~(1ULL << indexSQ(sq));
    // }

    inline constexpr bool checkSQ(SQ sq) const {
        return (chunks[sq.chunk()] >> sq.index()) & 0x1;
    }

    template<int s>
    inline void shift();

    inline void byteswap256();

    template<std::size_t chunk>
    inline void minus(SQ sq);

    template<std::size_t chunk>
    inline void hyperbola_quintessence(SQ sq, const BB& all, const auto& msk);

    template<std::size_t chunk>
    inline void obstruction_difference(SQ sq, const BB& all, const auto& msk, auto l, auto u);

    constexpr inline bool empty() const {
        return (chunks[0] | chunks[1] | chunks[2] | chunks[3]) == 0;
    }

    inline bool collision(const BB& bb) const
    {
        return (chunks[0] & bb.chunks[0]) ||
               (chunks[1] & bb.chunks[1]) ||
               (chunks[2] & bb.chunks[2]) ||
               (chunks[3] & bb.chunks[3]);
    }

    inline bool collision(const BB& bb1, const BB& bb2) const
    {
        return (chunks[0] & (bb1.chunks[0] | bb2.chunks[0])) ||
               (chunks[1] & (bb1.chunks[1] | bb2.chunks[1])) ||
               (chunks[2] & (bb1.chunks[2] | bb2.chunks[2])) ||
               (chunks[3] & (bb1.chunks[3] | bb2.chunks[3]));
    }

    void print(bool debug = true) const;
};

constexpr BB BLANK(0ULL, 0ULL, 0ULL, 0ULL);

// inline uint64_t byteswap64(uint64_t x)
// {
//     return __builtin_bswap64(x);

// // #if defined(__GNUC__) || defined(__clang__)
    
// // #else
// //     x = (x >> 32) | (x << 32);
// //     x = ((x & 0xFFFF0000FFFF0000ULL) >> 16) | ((x & 0x0000FFFF0000FFFFULL) << 16);
// //     x = ((x & 0xFF00FF00FF00FF00ULL) >> 8) | ((x & 0x00FF00FF00FF00FFULL) << 8);
// //     return x;
// // #endif
// }

inline u64 swap16(u64 x) {
    return ((x & 0x000000000000FFFFULL) << 48) |
           ((x & 0x00000000FFFF0000ULL) << 16) |
           ((x & 0x0000FFFF00000000ULL) >> 16) |
           ((x & 0xFFFF000000000000ULL) >> 48);
}

inline void BB::byteswap256()
{
    // const uint64_t tmp0 = chunks[0];
    // const uint64_t tmp1 = chunks[1];

    // chunks[0] = chunks[3];
    // chunks[1] = chunks[2];
    // chunks[3] = tmp0;
    // chunks[2] = tmp1;

    // std::cout << (chunks[0]) << " " << byteswap64(chunks[0]) << std::endl;

    const u64 tmp0 = swap16(chunks[0]);
    const u64 tmp1 = swap16(chunks[1]);

    chunks[0] = swap16(chunks[3]);
    chunks[1] = swap16(chunks[2]);
    chunks[3] = tmp0;
    chunks[2] = tmp1;
}

// inline void byteswap256(BitBoard &bb)
// {
//     uint64_t tmp0 = byteswap64(bb.chunks[0]);
//     uint64_t tmp1 = byteswap64(bb.chunks[1]);
//     bb.chunks[0] = byteswap64(bb.chunks[3]);
//     bb.chunks[1] = byteswap64(bb.chunks[2]);
//     bb.chunks[3] = tmp0;
//     bb.chunks[2] = tmp1;
// }


// inline void byteswap256(BB& bb)
// {
//     const uint64_t tmp0 = byteswap64(bb.chunks[0]);
//     const uint64_t tmp1 = byteswap64(bb.chunks[1]);

//     bb.chunks[0] = byteswap64(bb.chunks[3]);
//     bb.chunks[1] = byteswap64(bb.chunks[2]);
//     bb.chunks[2] = tmp1;
//     bb.chunks[3] = tmp0;
// }

template<std::size_t chunk>
inline void BB::minus(SQ sq)
{
    // auto chunk = curr;
    // if (swap)
    // {
    //     this->byteswap256();
    //     sq = sq.mirrorRank();
    //     chunk = 3 - chunk;
    // }

    // this->print();
    // std::cout << "c: " << (int)chunk << std::endl;
    // std::cout << "S: " << (int)sq.encoded << std::endl;

    // print();

    auto bb = (1ULL << sq.index()); // std::cout << "sq: " << (int)sq.encoded << " index: " << sq.index() << std::endl;
    bool b1 = ((chunk == 0) && (this->chunks[0] < bb));
    bool b2 = ((chunk == 1) && (this->chunks[1] < bb)) || ((chunk != 1) && b1 && (this->chunks[1] == 0));
    bool b3 = ((chunk == 2) && (this->chunks[2] < bb)) || ((chunk != 2) && b2 && (this->chunks[2] == 0));

    this->chunks[chunk] -= bb;
    this->chunks[1] -= b1;
    this->chunks[2] -= b2;
    this->chunks[3] -= b3;

    // print();

    // if (swap) this->byteswap256();
}

// template<std::size_t chunk>
// inline void minus(BB& bb, SQ sq)
// {
//     auto s = (1ULL << indexSQ(sq));

//     bool b1 = ((chunk == 0) && (bb.chunks[0] < s));
//     bool b2 = ((chunk == 1) && (bb.chunks[1] < s)) || ((chunk != 1) && b1 && (bb.chunks[1] == 0));
//     bool b3 = ((chunk == 2) && (bb.chunks[2] < s)) || ((chunk != 2) && b2 && (bb.chunks[2] == 0));

//     bb.chunks[chunk] = bb.chunks[chunk] - s;

//     bb.chunks[1] = bb.chunks[1] - (static_cast<uint64_t>(b1));
//     bb.chunks[2] = bb.chunks[2] - (static_cast<uint64_t>(b2));
//     bb.chunks[3] = bb.chunks[3] - (static_cast<uint64_t>(b3));
// }

template<std::size_t chunk>
inline void BB::hyperbola_quintessence(SQ sq, const BB& all, const auto& msk)
{
    BB bb1(all); bb1 &= msk;
    BB bb2(bb1);

    bb1.minus<    chunk>(sq); // bb2.print();
    bb2.byteswap256(); // bb2.print();
    bb2.minus<3 - chunk>(sq.flipRANK()); // bb2.print();
    bb2.byteswap256(); // bb2.print();
    bb1 ^= bb2; // bb1.print();
    bb1 &= msk;

    (*this) ^= bb1; //(*this).print();
    // (*this) &= msk;

    // print();

    // std::cout << "X xxxxxxxxxxxxxxxxxx" << std::endl;

    // attacks = all;
    // attacks &= mask;
    // BB temp(attacks);

    // minus<chunk>(attacks, sq);

    // byteswap256(temp);
    // minus<3 - chunk>(temp, mirrorRank(sq));
    // byteswap256(temp);

    // attacks ^= temp;
    // attacks &= mask;
}

template<int s>
inline void BB::shift()
{
    constexpr bool b = (s == +1) || (s == -1);
    if constexpr (s > 0)
    {
        chunks[3] = (chunks[3] << s) | (b ? 0ULL : (chunks[2] >> (64 - s)));
        chunks[2] = (chunks[2] << s) | (b ? 0ULL : (chunks[1] >> (64 - s)));
        chunks[1] = (chunks[1] << s) | (b ? 0ULL : (chunks[0] >> (64 - s)));
        chunks[0] = (chunks[0] << s);
    }
    else
    {
        constexpr auto rs = -s;
        chunks[0] = (chunks[0] >> rs) | (b ? 0ULL : (chunks[1] << (64 - rs)));
        chunks[1] = (chunks[1] >> rs) | (b ? 0ULL : (chunks[2] << (64 - rs)));
        chunks[2] = (chunks[2] >> rs) | (b ? 0ULL : (chunks[3] << (64 - rs)));
        chunks[3] = (chunks[3] >> rs);
    }
}

// template <Color color, int idx>
// inline void shift(BB &bb)
// {
//     constexpr auto s = PAWN_DELTA[color][idx];
//     constexpr bool b = (s == E) || (s == W);

//     if constexpr (s > 0)
//     {
//         bb.chunks[3] = (bb.chunks[3] << s) | (b ? 0ULL : (bb.chunks[2] >> (64 - s)));
//         bb.chunks[2] = (bb.chunks[2] << s) | (b ? 0ULL : (bb.chunks[1] >> (64 - s)));
//         bb.chunks[1] = (bb.chunks[1] << s) | (b ? 0ULL : (bb.chunks[0] >> (64 - s)));
//         bb.chunks[0] = (bb.chunks[0] << s);
//     }
//     else
//     {
//         constexpr auto rs = static_cast<std::size_t>(-s);
//         bb.chunks[0] = (bb.chunks[0] >> rs) | (b ? 0ULL : (bb.chunks[1] << (64 - rs)));
//         bb.chunks[1] = (bb.chunks[1] >> rs) | (b ? 0ULL : (bb.chunks[2] << (64 - rs)));
//         bb.chunks[2] = (bb.chunks[2] >> rs) | (b ? 0ULL : (bb.chunks[3] << (64 - rs)));
//         bb.chunks[3] = (bb.chunks[3] >> rs);
//     }
// }

// template <std::size_t chunk>
// inline void obstruction_difference(Square sq, const BB &occupied, BB &attacks)
// {
//     auto index = indexSQ(sq);
//     auto lower = LANE_MASK[index].lower & occupied.chunks[chunk];
//     auto upper = LANE_MASK[index].upper & occupied.chunks[chunk];

//     auto ms1B = 0x8000000000000000 >> __builtin_clzll(lower | 1);

//     auto odiff = upper ^ (upper - ms1B);
//     attacks.chunks[chunk] |= HORIZONTAL_ATTACKS[sq].chunks[chunk] & odiff;
// }

template<std::size_t chunk>
inline void BB::obstruction_difference(SQ sq, const BB& all, const auto& msk, auto l, auto u)
{
    auto index = sq.index();
    auto lower = l & all.chunks[chunk];
    auto upper = u & all.chunks[chunk];
    auto ms1B = 0x8000000000000000 >> __builtin_clzll(lower | 1);
    this->chunks[chunk] ^= (msk & (upper ^ (upper - ms1B)));
}




struct alignas(CACHELINE_SIZE) RightsInfo
{
    BB pass;
    SQ safe1, safe2;
    uint8_t padding[30];
};

// extern const std::array<RightsInfo, CASTLE_NB> RIGHTS_INFO;

struct CastleInfo
{
    SQ king_source;
    SQ king_target;
    SQ rook_source;
    SQ rook_target;
};

// extern constexpr std::array<CastleInfo, CASTLE_NB> CASTLE_INFO;

struct alignas(CACHELINE_SIZE) PawnInfo
{
    BB promotes;
    BB homerank;
};

// extern constexpr std::array<PawnInfo, (COLOR_NB - 4)> PAWN_INFO;

class DeltaInfo
{
    public:
    int pushS; // Single
    int pushD; // Double
    int takeE; // East
    int takeW; // West
};

alignas(CACHELINE_SIZE) inline constexpr std::array<DeltaInfo, PLAYER_NB> DELTA_INFO
{{
    { +16, +32, +17, +15 },
    { +1 , +2 , -15, +17 },
    { -1 , -2 , +15, -17 },
    { -16, -32, -15, -17 },
}};


consteval BB buildBB(std::initializer_list<Square> squares)
{
    BB bb;
    for (auto sq : squares) bb.toggleSQ(sq);
    return bb;
}

inline constexpr std::array<RightsInfo, CASTLE_NB> RIGHTS_INFO =
{{
    {{ buildBB({SQ::J2 , SQ::K2 }) }, SQ::I2 , SQ::J2 , {} }, {{ buildBB({SQ::F2 , SQ::G2 , SQ::H2 }) }, SQ::I2 , SQ::H2 , {} },
    {{ buildBB({SQ::B6 , SQ::B7 }) }, SQ::B8 , SQ::B7 , {} }, {{ buildBB({SQ::B9 , SQ::B10, SQ::B11}) }, SQ::B8 , SQ::B9 , {} },
    {{ buildBB({SQ::F15, SQ::G15}) }, SQ::H15, SQ::G15, {} }, {{ buildBB({SQ::I15, SQ::J15, SQ::K15}) }, SQ::H15, SQ::I15, {} },
    {{ buildBB({SQ::O10, SQ::O11}) }, SQ::O9 , SQ::O10, {} }, {{ buildBB({SQ::O6 , SQ::O7 , SQ::O8 }) }, SQ::O9 , SQ::O8 , {} },
    {{ buildBB({SQ::J2 , SQ::K2 }) }, SQ::I2 , SQ::J2 , {} }, {{ buildBB({SQ::F2 , SQ::G2 , SQ::H2 }) }, SQ::I2 , SQ::H2 , {} },
    {{ buildBB({SQ::B10, SQ::B11}) }, SQ::B9 , SQ::B10, {} }, {{ buildBB({SQ::B6 , SQ::B7 , SQ::B8 }) }, SQ::B9 , SQ::B8 , {} },
    {{ buildBB({SQ::F15, SQ::G15}) }, SQ::H15, SQ::G15, {} }, {{ buildBB({SQ::I15, SQ::J15, SQ::K15}) }, SQ::H15, SQ::I15, {} },
    {{ buildBB({SQ::O6 , SQ::O7 }) }, SQ::O8 , SQ::O7 , {} }, {{ buildBB({SQ::O9 , SQ::O10, SQ::O11}) }, SQ::O8 , SQ::O9 , {} },
}};

alignas(CACHELINE_SIZE)
inline constexpr std::array<CastleInfo, CASTLE_NB> CASTLE_INFO =
{{
    { SQ::I2 , SQ::K2 , SQ::L2 , SQ::J2 },  { SQ::I2 , SQ::G2 , SQ::E2 , SQ::H2 }, 
    { SQ::B8 , SQ::B6 , SQ::B5 , SQ::B7 },  { SQ::B8 , SQ::B10, SQ::B12, SQ::B9 },
    { SQ::H15, SQ::F15, SQ::E15, SQ::G15},  { SQ::H15, SQ::J15, SQ::L15, SQ::I15}, 
    { SQ::O9 , SQ::O11, SQ::O12, SQ::O10},  { SQ::O9 , SQ::O7 , SQ::O5 , SQ::O8 },
    { SQ::I2 , SQ::K2 , SQ::L2 , SQ::J2 },  { SQ::I2 , SQ::G2 , SQ::E2 , SQ::H2 }, 
    { SQ::B9 , SQ::B11, SQ::B12, SQ::B10},  { SQ::B9 , SQ::B7 , SQ::B5 , SQ::B8 },
    { SQ::H15, SQ::F15, SQ::E15, SQ::G15},  { SQ::H15, SQ::J15, SQ::L15, SQ::I15}, 
    { SQ::O8 , SQ::O6 , SQ::O5 , SQ::O7 },  { SQ::O8 , SQ::O10, SQ::O12, SQ::O9 },
}};

inline constexpr std::array<PawnInfo, (COLOR_NB - 4)> PAWN_INFO =
{{
    {buildBB({SQ::B11, SQ::C11, SQ::D11, SQ::E11, SQ::F11, SQ::G11, SQ::H11, SQ::I11, SQ::J11, SQ::K11, SQ::L11, SQ::M11, SQ::N11, SQ::O11}), buildBB({SQ::E3 , SQ::F3 , SQ::G3 , SQ::H3 , SQ::I3 , SQ::J3 , SQ::K3 , SQ::L3 , SQ::E4 , SQ::F4 , SQ::G4 , SQ::H4 , SQ::I4 , SQ::J4 , SQ::K4 , SQ::L4 })},
    {buildBB({SQ::K2 , SQ::K3 , SQ::K4 , SQ::K5 , SQ::K6 , SQ::K7 , SQ::K8 , SQ::K9 , SQ::K10, SQ::K11, SQ::K12, SQ::K13, SQ::K14, SQ::K15}), buildBB({SQ::C5 , SQ::C6 , SQ::C7 , SQ::C8 , SQ::C9 , SQ::C10, SQ::C11, SQ::C12, SQ::D5 , SQ::D6 , SQ::D7 , SQ::D8 , SQ::D9 , SQ::D10, SQ::D11, SQ::D12})},
    {buildBB({SQ::B6 , SQ::C6 , SQ::D6 , SQ::E6 , SQ::F6 , SQ::G6 , SQ::H6 , SQ::I6 , SQ::J6 , SQ::K6 , SQ::L6 , SQ::M6 , SQ::N6 , SQ::O6 }), buildBB({SQ::E14, SQ::F14, SQ::G14, SQ::H14, SQ::I14, SQ::J14, SQ::K14, SQ::L14, SQ::E13, SQ::F13, SQ::G13, SQ::H13, SQ::I13, SQ::J13, SQ::K13, SQ::L13})},
    {buildBB({SQ::F2 , SQ::F3 , SQ::F4 , SQ::F5 , SQ::F6 , SQ::F7 , SQ::F8 , SQ::F9 , SQ::F10, SQ::F11, SQ::F12, SQ::F13, SQ::F14, SQ::F15}), buildBB({SQ::N5 , SQ::N6 , SQ::N7 , SQ::N8 , SQ::N9 , SQ::N10, SQ::N11, SQ::N12, SQ::M5 , SQ::M6 , SQ::M7 , SQ::M8 , SQ::M9 , SQ::M10, SQ::M11, SQ::M12})},
}};


struct alignas(CACHELINE_SIZE) Straight
{
    BB  verti;
    u64 horiz;
    u64 lower; 
    u64 upper;
    u8  padding[8];
};

struct alignas(CACHELINE_SIZE) Diagonal
{
    BB diag;
    BB anti;
};

struct alignas(CACHELINE_SIZE) Adjacent
{
    BB king;
    BB knight;
};

extern const std::array<Straight, BOARDSIZE> STRAIGHT;
extern const std::array<Diagonal, BOARDSIZE> DIAGONAL;
extern const std::array<Adjacent, BOARDSIZE> ADJACENT;

template<std::size_t chunk, Piece piece>
inline BB& genSlideAttacks(SQ sq, const BB& all, BB& attacks)
{
    static_assert(chunk == 0 || chunk == 1 || 
                  chunk == 2 || chunk == 3, "genSlideAttacks: invalid chunk");

    static_assert(piece.encoded == Piece::R || piece.encoded == Piece::B,
                 "genSlideAttacks: invalid piece");

    if constexpr (piece.encoded == Piece::R)
    {
        const auto& [verti, horiz, lower, upper, padding] = STRAIGHT[compress<chunk>(sq).encoded];
        attacks.hyperbola_quintessence<chunk>(sq, all, verti);
        attacks.obstruction_difference<chunk>(sq, all, horiz, lower, upper);
    } 

    if constexpr (piece.encoded == Piece::B)
    {
        const auto& [diag, anti] = DIAGONAL[compress<chunk>(sq).encoded];
        attacks.hyperbola_quintessence<chunk>(sq, all, diag);
        attacks.hyperbola_quintessence<chunk>(sq, all, anti);
    }

    return attacks;
}

} // namespace athena

#endif /* BITBOARD_H */