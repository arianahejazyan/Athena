#ifndef BITBOARD_H
#define BITBOARD_H

#include "chess.h"

namespace athena
{

class BitBoard
{
    private:

        uint64_t chunks[4];

    public:

        constexpr BitBoard() noexcept
            : chunks{0, 0, 0, 0} {}

        constexpr BitBoard(uint64_t a, uint64_t b, uint64_t c, uint64_t d) noexcept
            : chunks{a, b, c, d} {}

        consteval BitBoard(std::initializer_list<Square> squares) noexcept
            : chunks{0, 0, 0, 0} { for (auto sq: squares) setSQ(sq); }

        constexpr inline BitBoard operator~() const noexcept
        {
            return BitBoard(
                ~chunks[0],
                ~chunks[1],
                ~chunks[2],
                ~chunks[3]
            );
        }

        constexpr inline BitBoard operator&(const BitBoard& other) const noexcept
        {
            return BitBoard(
                chunks[0] & other.chunks[0],
                chunks[1] & other.chunks[1],
                chunks[2] & other.chunks[2],
                chunks[3] & other.chunks[3]
            );
        }

        constexpr inline BitBoard operator|(const BitBoard& other) const noexcept
        {
            return BitBoard(
                chunks[0] | other.chunks[0],
                chunks[1] | other.chunks[1],
                chunks[2] | other.chunks[2],
                chunks[3] | other.chunks[3]
            );
        }

        constexpr inline BitBoard operator^(const BitBoard& other) const noexcept
        {
            return BitBoard(
                chunks[0] ^ other.chunks[0],
                chunks[1] ^ other.chunks[1],
                chunks[2] ^ other.chunks[2],
                chunks[3] ^ other.chunks[3]
            );
        }

        constexpr inline BitBoard& operator&=(const BitBoard& other) noexcept
        {
            chunks[0] &= other.chunks[0];
            chunks[1] &= other.chunks[1];
            chunks[2] &= other.chunks[2];
            chunks[3] &= other.chunks[3];
            return *this;
        }

        constexpr inline BitBoard& operator|=(const BitBoard& other) noexcept
        {
            chunks[0] |= other.chunks[0];
            chunks[1] |= other.chunks[1];
            chunks[2] |= other.chunks[2];
            chunks[3] |= other.chunks[3];
            return *this;
        }

        constexpr inline BitBoard& operator^=(const BitBoard& other) noexcept
        {
            chunks[0] ^= other.chunks[0];
            chunks[1] ^= other.chunks[1];
            chunks[2] ^= other.chunks[2];
            chunks[3] ^= other.chunks[3];
            return *this;
        }

        constexpr inline bool operator==(const BitBoard& other) const noexcept
        {
            return (chunks[0] == other.chunks[0]) && 
                   (chunks[1] == other.chunks[1]) && 
                   (chunks[2] == other.chunks[2]) && 
                   (chunks[3] == other.chunks[3]);
        }

        constexpr inline bool operator!=(const BitBoard& other) const noexcept {
            return !(*this == other);
        }

        constexpr inline void setSQ(int r, int f) noexcept
        {
            uint8_t s = r * 16 + f;
            uint8_t chunk = s >> 6;
            uint8_t index = s & 63;
            chunks[chunk] |= (1ULL << index);
        }

        constexpr inline void setSQ(Square sq) noexcept
        {
            uint8_t s = static_cast<uint8_t>(sq);
            uint8_t chunk = s >> 6;
            uint8_t index = s & 63;
            chunks[chunk] |= (1ULL << index);
        }

        constexpr inline void popSQ(Square sq) noexcept
        {
            uint8_t s = static_cast<uint8_t>(sq);
            uint8_t chunk = s >> 6;
            uint8_t index = s & 63;
            chunks[chunk] &= ~(1ULL << index);
        }

        constexpr inline bool checkSQ(Square sq) const noexcept
        {
            uint8_t s = static_cast<uint8_t>(sq);
            uint8_t chunk = s >> 6;
            uint8_t index = s & 63;
            return (chunks[chunk] & (1ULL << index)) != 0;
        }

        constexpr inline int popCount() const noexcept
        {
            return __builtin_popcountll(chunks[0]) + 
                   __builtin_popcountll(chunks[1]) + 
                   __builtin_popcountll(chunks[2]) + 
                   __builtin_popcountll(chunks[3]);
        }

        inline Square popLSB() noexcept 
        {
            uint32_t mask = (chunks[0] != 0) | ((chunks[1] != 0) << 1) | ((chunks[2] != 0) << 2) | ((chunks[3] != 0) << 3);
            int chunk_idx = __builtin_ctz(mask);
            uint64_t bits = chunks[chunk_idx];
            chunks[chunk_idx] = bits & (bits - 1);
            return static_cast<Square>((chunk_idx << 6) + __builtin_ctzll(bits));
        }

        inline Square lsb() const noexcept
        {
            uint32_t mask = (chunks[0] != 0) | ((chunks[1] != 0) << 1) | ((chunks[2] != 0) << 2) | ((chunks[3] != 0) << 3);
            int chunk_idx = __builtin_ctz(mask);
            uint64_t bits = chunks[chunk_idx];
            return static_cast<Square>((chunk_idx << 6) + __builtin_ctzll(bits));
        }

        constexpr operator bool() const noexcept {
            return chunks[0] | chunks[1] | chunks[2] | chunks[3];
        }

        inline BitBoard shift(Shift s) const noexcept;

        constexpr uint64_t& chunk(int idx) noexcept { return chunks[idx]; }

        // Forward declaration
        class Iterator; 

        Iterator begin() const;
        Iterator end() const;
};

// === Iterator class ===
class BitBoard::Iterator {
private:
    BitBoard bb;

public:
    explicit Iterator(BitBoard b) : bb(b) {}

    Square operator*() const {
        BitBoard temp = bb;
        return temp.popLSB();
    }

    Iterator& operator++() {
        bb.popLSB();
        return *this;
    }

    bool operator!=(const Iterator& other) const {
        return static_cast<bool>(bb) != static_cast<bool>(other.bb);
    }
};

// === Iterator implementation ===
inline BitBoard::Iterator BitBoard::begin() const {
    return Iterator(*this);
}

inline BitBoard::Iterator BitBoard::end() const {
    return Iterator(BitBoard());
}

// Choosing alias
using BB = BitBoard;

constexpr BitBoard BLANK = BitBoard(0, 0, 0, 0);
constexpr BitBoard BRICK = []() consteval
{
    BitBoard bb {};
    for (auto sq: ALL_SQUARES)
        if (isValidSquare(rankSQ(sq), fileSQ(sq)) == false) bb.setSQ(sq);
    return bb;
}();

inline BitBoard BitBoard::shift(Shift s) const noexcept 
{
    BitBoard result;
    if (s > 0)
    {
        result.chunks[0] = (chunks[0] << s);
        result.chunks[1] = (chunks[1] << s) | (chunks[0] >> (64 - s));
        result.chunks[2] = (chunks[2] << s) | (chunks[1] >> (64 - s));
        result.chunks[3] = (chunks[3] << s) | (chunks[2] >> (64 - s));
    } 
    else 
    {
        s = static_cast<Shift>(-s);
        result.chunks[0] = (chunks[0] >> s) | (chunks[1] << (64 - s));
        result.chunks[1] = (chunks[1] >> s) | (chunks[2] << (64 - s));
        result.chunks[2] = (chunks[2] >> s) | (chunks[3] << (64 - s));
        result.chunks[3] = (chunks[3] >> s);
    }
    return result & ~BRICK;
}

template<auto... D>
consteval auto genMask(bool slide)
{
    static_assert(sizeof...(D) % 2 == 0, "Deltas must be in (dr, df) pairs");

    std::array<BitBoard, SQUARE_NB> arr{};

    constexpr int deltas[]  = { D... };
    constexpr int numDeltas = sizeof...(D) / 2;

    for (Square sq : VALID_SQUARES)
    {
        int r = rankSQ(sq);
        int f = fileSQ(sq);

        for (int i = 0; i < numDeltas; ++i)
        {
            int dr = deltas[2 * i];
            int df = deltas[2 * i + 1];
            int tr = r + dr;
            int tf = f + df;

            while (isValidSquare(tr, tf))
            {
                arr[sq].setSQ(tr, tf);
                if (!slide) break;
                tr += dr;
                tf += df;
            }
        }
    }

    return arr;
}

// visually grouping coordinate pairs
#define P(x, y) static_cast<Offset>((x)), static_cast<Offset>((y))

alignas(64) inline constexpr auto PIECE_ATTACK = []() consteval 
{
    std::array<std::array<BitBoard, SQUARE_NB>, PIECE_NB - 4> arr{};

    arr[King]   = genMask< P(U,R), P(U,F), P(U,L), P(F,R), P(F,L), P(D,R), P(D,F), P(D,L) >(false);
    arr[Rook]   = genMask< P(U,F), P(F,R), P(F,L), P(D,F) >(true);
    arr[Bishop] = genMask< P(U,R), P(U,L), P(D,R), P(D,L) >(true);
    arr[Knight] = genMask<
        P(U + U, R), P(U + U, L),
        P(U, R + R), P(U, L + L),
        P(D, R + R), P(D, L + L),
        P(D + D, R), P(D + D, L)
    >(false);

    return arr;
}();

alignas(64) inline constexpr std::array<std::array<BitBoard, SQUARE_NB>, COLOR_NB - 1> COLOR_ATTACK = 
{
    genMask<P(U, R), P(U, L)>(false),
    genMask<P(U, R), P(D, R)>(false),
    genMask<P(D, R), P(D, L)>(false),
    genMask<P(U, L), P(D, L)>(false),
};

inline auto between(Square a, Square b, Piece piece) noexcept
{
    static constexpr std::array<std::array<BitBoard, CHUNK_NB>, CHUNK_NB> BETWEEN_MASK = 
    {{{
        BitBoard(0x0ff00ff00ff00000ULL, 0ULL, 0ULL, 0ULL), // [0,0]
        BitBoard(0x0ff00ff00ff00000ULL, 0x7ffe7ffe7ffe7ffeULL, 0ULL, 0ULL), // [0,1]
        BitBoard(0x0ff00ff00ff00000ULL, 0x7ffe7ffe7ffe7ffeULL, 0x7ffe7ffe7ffe7ffeULL, 0ULL), // [0,2]
        BitBoard(0x0ff00ff00ff00000ULL, 0x7ffe7ffe7ffe7ffeULL, 0x7ffe7ffe7ffe7ffeULL, 0x00000ff00ff00ff0ULL), // [0,3]
    },
    {
        BitBoard(0ULL, 0ULL, 0ULL, 0ULL), // [1,0]
        BitBoard(0ULL, 0x7ffe7ffe7ffe7ffeULL, 0ULL, 0ULL), // [1,1]
        BitBoard(0ULL, 0x7ffe7ffe7ffe7ffeULL, 0x7ffe7ffe7ffe7ffeULL, 0ULL), // [1,2]
        BitBoard(0ULL, 0x7ffe7ffe7ffe7ffeULL, 0x7ffe7ffe7ffe7ffeULL, 0x00000ff00ff00ff0ULL), // [1,3]
    },
    {
        BitBoard(0ULL, 0ULL, 0ULL, 0ULL), // [2,0]
        BitBoard(0ULL, 0ULL, 0ULL, 0ULL), // [2,1]
        BitBoard(0ULL, 0ULL, 0x7ffe7ffe7ffe7ffeULL, 0ULL), // [2,2]
        BitBoard(0ULL, 0ULL, 0x7ffe7ffe7ffe7ffeULL, 0x00000ff00ff00ff0ULL), // [2,3]
    },
    {
        BitBoard(0ULL, 0ULL, 0ULL, 0ULL), // [3,0]
        BitBoard(0ULL, 0ULL, 0ULL, 0ULL), // [3,1]
        BitBoard(0ULL, 0ULL, 0ULL, 0ULL), // [3,2]
        BitBoard(0ULL, 0ULL, 0ULL, 0x00000ff00ff00ff0ULL), // [3,3]
    }}};

    // Make sure a < b
    int swap = -(a > b);
    int xor_ab = a ^ b;
    a = static_cast<Square>(a ^ (xor_ab & swap));
    b = static_cast<Square>(b ^ (xor_ab & swap));

    int chunk_a = chunkSQ(a); // a / 64
    int chunk_b = chunkSQ(b); // b / 64
    int index_a = indexSQ(a); // a % 64
    int index_b = indexSQ(b); // b % 64

    auto betweenMask = BETWEEN_MASK[chunk_a][chunk_b]; 
    betweenMask.chunk(chunk_a) &= ~((1ULL << (index_a + 1)) - 1);
    betweenMask.chunk(chunk_b) &=  ((1ULL << index_b) - 1);

    return betweenMask & PIECE_ATTACK[piece][a] & PIECE_ATTACK[piece][b];
}

inline bool hasCastle(BB castle, BB right) noexcept {
    return (castle & right) == (right);
}

constexpr ndarray<BB, GAMESETUP_NB, COLOR_NB - 1, SIDE_NB> RIGHTS =
{{{{
    { BB({I2 , L2 }), BB({I2 , E2 }) },   
    { BB({B9 , B12}), BB({B9 , B5 }) },  
    { BB({H15, E15}), BB({H15, L15}) },
    { BB({O8 , O5 }), BB({O8 , O12}) }, 
}},{{
    { BB({I2 , L2 }), BB({I2 , E2 }) },
    { BB({B8 , B5 }), BB({B8 , B12}) },
    { BB({H15, E15}), BB({H15, L15}) },
    { BB({O9 , O12}), BB({O9 , O5 }) },
}}}};

constexpr ndarray<BB, GAMESETUP_NB, COLOR_NB - 1, SIDE_NB> PASS =
{{{{
    { BB({J2 , K2 }), BB({F2 , G2 , H2 }) },
    { BB({B10, B11}), BB({B6 , B7 , B8 }) },
    { BB({F15, G15}), BB({I15, J15, K15}) },
    { BB({O6 , O7 }), BB({O9 , O10, O11}) },
}},{{
    { BB({J2 , K2 }), BB({F2 , G2 , H2 }) },
    { BB({B6 , B7 }), BB({B9 , B10, B11}) },
    { BB({F15, G15}), BB({I15, J15, K15}) },
    { BB({O10, O11}), BB({O6 , O7 , O8 }) },
}}}};

constexpr ndarray<BB, GAMESETUP_NB, COLOR_NB - 1, SIDE_NB> SAFE =
{{{{
    { BB({I2 , J2 }), BB({I2 , H2 }) },
    { BB({B9 , B10}), BB({B9 , B8 }) },
    { BB({H15, G15}), BB({H15, I15}) },
    { BB({O8 , O7 }), BB({O8 , O9 }) },
}},{{
    { BB({I2 , J2 }), BB({I2 , H2 }) },
    { BB({B8 , B7 }), BB({B8 , B9 }) },
    { BB({H15, G15}), BB({H15, I15}) },
    { BB({O9 , O10}), BB({O9 , O8 }) },
}}}};

constexpr ndarray<Square, GAMESETUP_NB, COLOR_NB - 1, SIDE_NB> SOURCE_CASTLE =
{{{{
    { L2 , E2  },
    { B12, B5  },
    { E15, L15 },
    { O5 , O12 },
}},{{
    { L2 , E2  },
    { B5 , B12 },
    { E15, L15 },
    { O12, O5  },
}}}};

constexpr ndarray<Square, GAMESETUP_NB, COLOR_NB - 1, SIDE_NB> TARGET_CASTLE =
{{{{
    { J2 , H2  },
    { B10, B8  },
    { G15, I15 },
    { O7 , O9  },
}},{{
    { J2 , H2  },
    { B7 , B9  },
    { G15, I15 },
    { O10, O8  },
}}}};

constexpr auto PROMOTE = []() consteval 
{
    ndarray<BB, COLOR_NB - 1> arr {};
    arr[Red]    = BB({ B11, C11, D11, E11, F11, G11, H11, I11, J11, K11, L11, M11, N11, O11 });
    arr[Blue]   = BB({ K2 , K3 , K4 , K5 , K6 , K7 , K8 , K9 , K10, K11, K12, K13, K14, K15 });
    arr[Yellow] = BB({ B6 , C6 , D6 , E6 , F6 , G6 , H6 , I6 , J6 , K6 , L6 , M6 , N6 , O6  });
    arr[Green]  = BB({ F2 , F3 , F4 , F5 , F6 , F7 , F8 , F9 , F10, F11, F12, F13, F14, F15 });
    return arr;
}();

constexpr auto HOMERANK = []() consteval
{
    ndarray<BB, COLOR_NB - 1> arr {};
    arr[Red]    = BB({ E3 , F3 , G3 , H3 , I3 , J3 , K3 , L3 , E4 , F4 , G4 , H4 , I4 , J4 , K4 , L4  });
    arr[Blue]   = BB({ C5 , C6 , C7 , C8 , C9 , C10, C11, C12, D5 , D6 , D7 , D8 , D9 , D10, D11, D12 });
    arr[Yellow] = BB({ E14, F14, G14, H14, I14, J14, K14, L14, E13, F13, G13, H13, I13, J13, K13, L13 });
    arr[Green]  = BB({ N5 , N6 , N7 , N8 , N9 , N10, N11, N12, M5 , M6 , M7 , M8 , M9 , M10, M11, M12 });
    return arr;
}();

} // namespace athena

#endif // #ifndef BITBOARD_H