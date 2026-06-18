#pragma once

#include <iostream>
#include <cstdint>
#include <immintrin.h>
#include "constants.h"
#include "square.h"
#include "color.h"

namespace athena::chess {

class alignas(32) Bitboard {
public:
    using Chunk   = uint64_t;
    using ChunkID = uint8_t;

    constexpr Bitboard() noexcept = default;
    constexpr Bitboard(Chunk c) noexcept : chunks_{c, c, c, c} {}
    constexpr Bitboard(Chunk a, Chunk b, Chunk c, Chunk d) noexcept : chunks_{a, b, c, d} {}

    constexpr Bitboard& operator&=(const Bitboard& __restrict__ rhs) noexcept {
        chunks_[0] &= rhs.chunks_[0];
        chunks_[1] &= rhs.chunks_[1];
        chunks_[2] &= rhs.chunks_[2];
        chunks_[3] &= rhs.chunks_[3];
        return *this;
    }

    constexpr Bitboard& operator|=(const Bitboard& __restrict__ rhs) noexcept {
        chunks_[0] |= rhs.chunks_[0];
        chunks_[1] |= rhs.chunks_[1];
        chunks_[2] |= rhs.chunks_[2];
        chunks_[3] |= rhs.chunks_[3];
        return *this;
    }

    constexpr Bitboard& operator^=(const Bitboard& __restrict__ rhs) noexcept {
        chunks_[0] ^= rhs.chunks_[0];
        chunks_[1] ^= rhs.chunks_[1];
        chunks_[2] ^= rhs.chunks_[2];
        chunks_[3] ^= rhs.chunks_[3];
        return *this;
    }

    constexpr Bitboard& operator<<=(int shift) noexcept {
        chunks_[0] <<= shift;
        chunks_[1] <<= shift;
        chunks_[2] <<= shift;
        chunks_[3] <<= shift;
        return *this;
    }

    constexpr Bitboard& operator>>=(int shift) noexcept {
        chunks_[0] >>= shift;
        chunks_[1] >>= shift;
        chunks_[2] >>= shift;
        chunks_[3] >>= shift;
        return *this;
    }

    constexpr Bitboard operator&(const Bitboard& __restrict__ rhs) const noexcept {
        return {
            chunks_[0] & rhs.chunks_[0],
            chunks_[1] & rhs.chunks_[1],
            chunks_[2] & rhs.chunks_[2],
            chunks_[3] & rhs.chunks_[3],
        };
    }

    constexpr Bitboard operator|(const Bitboard& __restrict__ rhs) const noexcept {
        return {
            chunks_[0] | rhs.chunks_[0],
            chunks_[1] | rhs.chunks_[1],
            chunks_[2] | rhs.chunks_[2],
            chunks_[3] | rhs.chunks_[3],
        };
    }
    
    constexpr Bitboard operator^(const Bitboard& __restrict__ rhs) const noexcept {
        return {
            chunks_[0] ^ rhs.chunks_[0],
            chunks_[1] ^ rhs.chunks_[1],
            chunks_[2] ^ rhs.chunks_[2],
            chunks_[3] ^ rhs.chunks_[3],
        };
    }
     
    constexpr Bitboard operator<<(int shift) const noexcept {
        return {
            chunks_[0] << shift,
            chunks_[1] << shift,
            chunks_[2] << shift,
            chunks_[3] << shift,
        };
    }
    
    constexpr Bitboard operator>>(int shift) const noexcept {
        return {
            chunks_[0] >> shift,
            chunks_[1] >> shift,
            chunks_[2] >> shift,
            chunks_[3] >> shift,
        };
    }

    constexpr Bitboard operator~() const noexcept {
        return {
            ~chunks_[0],
            ~chunks_[1],
            ~chunks_[2],
            ~chunks_[3],
        };
    }
    
    constexpr bool has_bit(Square sq) const noexcept { 
        return (chunks_[sq.chunk()] >> sq.index()) & 0x1;
    }

    constexpr Bitboard& set_bit(Square sq) noexcept { 
        chunks_[sq.chunk()] |=  (1ULL << sq.index()); 
        return *this;
    }

    constexpr Bitboard& pop_bit(Square sq) noexcept { 
        chunks_[sq.chunk()] &= ~(1ULL << sq.index()); 
        return *this;
    }

    constexpr Chunk combine() const noexcept {
        return 
            chunks_[0] | 
            chunks_[1] | 
            chunks_[2] | 
            chunks_[3];
    }

    constexpr bool empty() const noexcept {
        return combine() == 0;
    }

    constexpr bool any() const noexcept {
        return combine() != 0;
    }

    int count() const noexcept {
        return __builtin_popcountll(chunks_[0])
             + __builtin_popcountll(chunks_[1])
             + __builtin_popcountll(chunks_[2])
             + __builtin_popcountll(chunks_[3]);
    }

    void swap_bytes() noexcept {
        std::swap(chunks_[0], chunks_[3]);
        std::swap(chunks_[1], chunks_[2]);
        chunks_[0] = __builtin_bswap64(chunks_[0]);
        chunks_[1] = __builtin_bswap64(chunks_[1]);
        chunks_[2] = __builtin_bswap64(chunks_[2]);
        chunks_[3] = __builtin_bswap64(chunks_[3]);
    }

    Chunk& chunk(ChunkID id) noexcept { return chunks_[id]; }
    const Chunk& chunk(ChunkID id) const noexcept { return chunks_[id]; }

    template<Square::Offset offset>
    void shift_east() noexcept {
        constexpr int bits = static_cast<int>(offset);
        constexpr int anti = CHUNK_SIZE - bits;
        const Chunk c0 = chunks_[0], c1 = chunks_[1], c2 = chunks_[2], c3 = chunks_[3];
        chunks_[0] = (c0 >> bits) | (c1 << anti);
        chunks_[1] = (c1 >> bits) | (c2 << anti);
        chunks_[2] = (c2 >> bits) | (c3 << anti);
        chunks_[3] =  c3 >> bits;
    }

    template<Square::Offset offset>
    void shift_west() noexcept {
        constexpr int bits = static_cast<int>(offset);
        constexpr int anti = CHUNK_SIZE - bits;
        const Chunk c0 = chunks_[0], c1 = chunks_[1], c2 = chunks_[2], c3 = chunks_[3];
        chunks_[0] =  c0 << bits;
        chunks_[1] = (c1 << bits) | (c0 >> anti);
        chunks_[2] = (c2 << bits) | (c1 >> anti);
        chunks_[3] = (c3 << bits) | (c2 >> anti);
    }

    template<Square::Offset offset>
    void shift() noexcept {
        if constexpr (offset > 0) shift_west<offset>();
        if constexpr (offset < 0) shift_east<Square::Offset(-static_cast<int>(offset))>();
    }

    template<ChunkID chunk_id>
    void ray_attacks_cross(Square::Index idx) noexcept;

    template<ChunkID chunk_id>
    void ray_attacks_chunk(Square::Index idx) noexcept;

    void print(bool board16x16 = true) const;

private:
    Chunk chunks_[CHUNK_NB];

public:
    static Square pop_lsb(Chunk& chunk, ChunkID chunk_id) noexcept {
        int bit = __builtin_ctzll(chunk);
        chunk &= chunk - 1;
        return static_cast<Square::ID>(static_cast<int>(chunk_id) * CHUNK_SIZE + bit);
    }
    
    static Square pop_msb(Chunk& chunk, ChunkID chunk_id) noexcept {
        int bit = 63 - __builtin_clzll(chunk);
        chunk &= ~(1ULL << bit);
        return static_cast<Square::ID>(static_cast<int>(chunk_id) * CHUNK_SIZE + bit);
    }

    static Square pop_lsb(Bitboard& bb) noexcept {
        for (int i = 0; i < 4; ++i) {
            if (bb.chunks_[i])
                return pop_lsb(bb.chunks_[i], i);
        }
        return Square::offboard();
    }
    
    static Square pop_msb(Bitboard& bb) noexcept {
        for (int i = 3; i >= 0; --i) {
            if (bb.chunks_[i])
                return pop_msb(bb.chunks_[i], i);
        }
        return Square::offboard();
    }

    static Square lsb(Chunk chunk, ChunkID chunk_id) noexcept {
        int bit = __builtin_ctzll(chunk);
        return static_cast<Square::ID>(static_cast<int>(chunk_id) * CHUNK_SIZE + bit);
    }
    
    static Square msb(Chunk chunk, ChunkID chunk_id) noexcept {
        int bit = 63 - __builtin_clzll(chunk);
        return static_cast<Square::ID>(static_cast<int>(chunk_id) * CHUNK_SIZE + bit);
    }
    
    static Square lsb(const Bitboard& bb) noexcept {
        for (int i = 0; i < 4; ++i) {
            if (bb.chunks_[i])
                return lsb(bb.chunks_[i], i);
        }
        return Square::offboard();
    }
    
    static Square msb(const Bitboard& bb) noexcept {
        for (int i = 3; i >= 0; --i) {
            if (bb.chunks_[i])
                return msb(bb.chunks_[i], i);
        }
        return Square::offboard();
    }

    static constexpr Bitboard homerank(Color::ID color) noexcept;
    static constexpr Bitboard promote (Color::ID color) noexcept;

    static constexpr Bitboard ones() noexcept { return Bitboard(UINT64_MAX);}
    static constexpr Bitboard zero() noexcept { return Bitboard(0ULL);}

    static constexpr Bitboard valid() noexcept;
    static constexpr Bitboard stone() noexcept;
};

consteval Bitboard makeRank(Square::Rank rank) noexcept { Bitboard bb{}; for (int f=0; f<FILE_NB; f++) { bb.set_bit(Square(Square::File(f), rank)); } return bb; }
consteval Bitboard makeFile(Square::File file) noexcept { Bitboard bb{}; for (int r=0; r<RANK_NB; r++) { bb.set_bit(Square(file, Square::Rank(r))); } return bb; }

alignas(64) inline constexpr 
std::array<std::pair<Bitboard, Bitboard>, COLOR_NB> 
TABLE_HOMERANK_PROMOTES = {{
    { makeRank(static_cast<Square::Rank>(          HOMERANK    )) | makeRank(static_cast<Square::Rank>(          HOMERANK + 1)), makeRank(static_cast<Square::Rank>(          PROMOTES    )) },
    { makeFile(static_cast<Square::File>(          HOMERANK    )) | makeFile(static_cast<Square::File>(          HOMERANK + 1)), makeFile(static_cast<Square::File>(          PROMOTES    )) },
    { makeRank(static_cast<Square::Rank>(RANK_NB - HOMERANK - 2)) | makeRank(static_cast<Square::Rank>(RANK_NB - HOMERANK - 1)), makeRank(static_cast<Square::Rank>(RANK_NB - PROMOTES - 1)) },
    { makeFile(static_cast<Square::File>(FILE_NB - HOMERANK - 2)) | makeFile(static_cast<Square::File>(FILE_NB - HOMERANK - 1)), makeFile(static_cast<Square::File>(FILE_NB - PROMOTES - 1)) },
}};  

inline constexpr Bitboard Bitboard::homerank(Color::ID color) noexcept { return TABLE_HOMERANK_PROMOTES[static_cast<uint8_t>(color)].first ; }
inline constexpr Bitboard Bitboard::promote (Color::ID color) noexcept { return TABLE_HOMERANK_PROMOTES[static_cast<uint8_t>(color)].second; }

inline constexpr Bitboard Bitboard::valid() noexcept {
    Bitboard bb {0ULL};
    for (int s=0; s<SQUARE_NB; s++) {
        auto sq = Square(static_cast<Square::ID>(s));
        if (sq.isValid()) bb.set_bit(sq); 
    }
    return bb;
}

inline constexpr Bitboard Bitboard::stone() noexcept {
    Bitboard bb {0ULL};
    for (int s=0; s<SQUARE_NB; s++) {
        auto sq = Square(static_cast<Square::ID>(s));
        if (sq.isStone()) bb.set_bit(sq); 
    }
    return bb;
}

inline Bitboard& operator-=(
    Bitboard& __restrict__ bitboard, Square sq) noexcept {
    Bitboard bb{};
    bb.set_bit(sq);
    uint64_t borrow; 
    borrow = __builtin_sub_overflow(bitboard.chunk(0), bb.chunk(0)         , &bitboard.chunk(0));
    borrow = __builtin_sub_overflow(bitboard.chunk(1), bb.chunk(1) + borrow, &bitboard.chunk(1));
    borrow = __builtin_sub_overflow(bitboard.chunk(2), bb.chunk(2) + borrow, &bitboard.chunk(2));
             __builtin_sub_overflow(bitboard.chunk(3), bb.chunk(3) + borrow, &bitboard.chunk(3));
    return bitboard;
}

// template<>
// inline void Bitboard::ray_attacks_cross<static_cast<Bitboard::ChunkID>(0)>(Square::Index idx) noexcept {
//     const uint64_t index = 1ULL << static_cast<uint8_t>(idx);
//     const uint64_t lower = (index - 1);
//     const uint64_t upper = ~lower;
//     uint64_t bu = index;
//     uint64_t bl = index;
//     bl = __builtin_bswap64(bl);
//     uint64_t u3, u2, u1, u0, l0;
//     u3 = chunks_[3];
//     u2 = chunks_[2];
//     u1 = chunks_[1];
//     u0 = chunks_[0] & upper;
//     l0 = chunks_[0] & lower;
//     l0 = __builtin_bswap64(l0);
//     bl = __builtin_sub_overflow(l0, bl, &l0);
//     bu = __builtin_sub_overflow(u0, bu, &chunks_[0]);
//     bu = __builtin_sub_overflow(u1, bu, &chunks_[1]);
//     bu = __builtin_sub_overflow(u2, bu, &chunks_[2]);
//          __builtin_sub_overflow(u3, bu, &chunks_[3]);
//     chunks_[0] |= __builtin_bswap64(l0);
// }

// template<>
// inline void Bitboard::ray_attacks_cross<static_cast<Bitboard::ChunkID>(1)>(Square::Index idx) noexcept {
//     const uint64_t index = 1ULL << static_cast<uint8_t>(idx);
//     const uint64_t lower = (index - 1);
//     const uint64_t upper = ~lower;
//     uint64_t bu = index;
//     uint64_t bl = index;
//     bl = __builtin_bswap64(bl);
//     uint64_t u3, u2, u1, l1, l0;
//     u3 = chunks_[3];
//     u2 = chunks_[2];
//     u1 = chunks_[1] & upper;
//     l1 = chunks_[1] & lower;
//     l0 = chunks_[0];
//     l1 = __builtin_bswap64(l1); 
//     l0 = __builtin_bswap64(l0); 
//     bl = __builtin_sub_overflow(l1, bl, &l1);
//     bl = __builtin_sub_overflow(l0, bl, &l0);
//     bu = __builtin_sub_overflow(u1, bu, &chunks_[1]);
//     bu = __builtin_sub_overflow(u2, bu, &chunks_[2]);
//          __builtin_sub_overflow(u3, bu, &chunks_[3]);
//     chunks_[0]  = __builtin_bswap64(l0);
//     chunks_[1] |= __builtin_bswap64(l1);
// }

// template<>
// inline void Bitboard::ray_attacks_cross<static_cast<Bitboard::ChunkID>(2)>(Square::Index idx) noexcept {
//     const uint64_t index = 1ULL << static_cast<uint8_t>(idx);
//     const uint64_t lower = (index - 1);
//     const uint64_t upper = ~lower;
//     uint64_t bu = index;
//     uint64_t bl = index;
//     bl = __builtin_bswap64(bl);
//     uint64_t u3, u2, l2, l1, l0;
//     u3 = chunks_[3];
//     u2 = chunks_[2] & upper;
//     l2 = chunks_[2] & lower;
//     l1 = chunks_[1];
//     l0 = chunks_[0];
//     l2 = __builtin_bswap64(l2);
//     l1 = __builtin_bswap64(l1);
//     l0 = __builtin_bswap64(l0);
//     bl = __builtin_sub_overflow(l2, bl, &l2);
//     bl = __builtin_sub_overflow(l1, bl, &l1); 
//     bl = __builtin_sub_overflow(l0, bl, &l0);         
//     bu = __builtin_sub_overflow(u2, bu, &chunks_[2]);
//          __builtin_sub_overflow(u3, bu, &chunks_[3]);
//     chunks_[0]  = __builtin_bswap64(l0);
//     chunks_[1]  = __builtin_bswap64(l1);
//     chunks_[2] |= __builtin_bswap64(l2);
// }

// template<>
// inline void Bitboard::ray_attacks_cross<static_cast<Bitboard::ChunkID>(3)>(Square::Index idx) noexcept {
//     const uint64_t index = 1ULL << static_cast<uint8_t>(idx);
//     const uint64_t lower = (index - 1);
//     const uint64_t upper = ~lower;
//     uint64_t bu = index;
//     uint64_t bl = index;
//     bl = __builtin_bswap64(bl);
//     uint64_t u3, l3, l2, l1, l0;
//     u3 = chunks_[3] & upper;
//     l3 = chunks_[3] & lower;
//     l2 = chunks_[2];
//     l1 = chunks_[1];
//     l0 = chunks_[0];
//     l3 = __builtin_bswap64(l3);
//     l2 = __builtin_bswap64(l2);
//     l1 = __builtin_bswap64(l1);
//     l0 = __builtin_bswap64(l0);
//     bl = __builtin_sub_overflow(l3, bl, &l3);
//     bl = __builtin_sub_overflow(l2, bl, &l2);
//     bl = __builtin_sub_overflow(l1, bl, &l1);
//     bl = __builtin_sub_overflow(l0, bl, &l0);
//          __builtin_sub_overflow(u3, bu, &chunks_[3]);
//     chunks_[0]  = __builtin_bswap64(l0);
//     chunks_[1]  = __builtin_bswap64(l1);
//     chunks_[2]  = __builtin_bswap64(l2);
//     chunks_[3] |= __builtin_bswap64(l3);
// }

// template<Bitboard::ChunkID chunk_id>
// inline void Bitboard::ray_attacks_chunk(
//     Square::Index idx) noexcept {
//     constexpr int shift_lsb = (chunk_id == 0 || chunk_id == 3) ? 12 : 15;
//     constexpr int shift_msb = (chunk_id == 0 || chunk_id == 3) ?  4 :  1;

//     const uint64_t index = 1ULL << static_cast<uint8_t>(idx);
//     const uint64_t lower = chunks_[chunk_id] & (index - 1);
//     const uint64_t upper = chunks_[chunk_id] & ~lower;

//     const auto lsb = 1  + __builtin_ctzll(upper | (1ULL << (shift_lsb + (static_cast<uint8_t>(idx) & ~0xFU))));
//     const auto msb = 63 - __builtin_clzll(lower | (1ULL << (shift_msb + (static_cast<uint8_t>(idx) & ~0xFU))));

//     chunks_[chunk_id] = (1ULL << lsb) - (1ULL << msb) - index;
// }

inline void Bitboard::print(bool board16x16) const {
    const int size   = board16x16 ? 16 : 14;
    const int offset = board16x16 ? 0  : 1 ;
    std::cout << "\n    ";
    for (int file = 0; file < size; ++file) std::cout << static_cast<char>('a' + file) << ' ';
    std::cout << "\n";
    for (int rank = size - 1; rank >= 0; --rank) {
        int row_label = rank + 1;
        std::cout << (row_label < 10 ? " " : "") << row_label << "  ";
        for (int file = 0; file < size; ++file) {
            int r = rank + offset;
            int f = file + offset;
            auto sq = Square(f, r);
            bool valid = sq.isStone();
            bool set = has_bit(sq);
            if (set) std::cout << (!valid ? "\033[34mX\033[0m" : "\033[90mX\033[0m");
            else     std::cout << (!valid ? "." : "\033[90m.\033[0m");
            if (file < size - 1) std::cout << ' ';
        }
        std::cout << "  " << (row_label < 10 ? " " : "") << row_label << '\n';
    }
    std::cout << "    ";
    for (int file = 0; file < size; ++file) std::cout << static_cast<char>('a' + file) << ' ';
    std::cout << "\n" << std::endl;
}

} // namespace athena