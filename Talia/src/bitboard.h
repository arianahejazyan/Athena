#ifndef BITBOARD_H
#define BITBOARD_H

// #include <cstdint>
// #include <sstream>
// #include <iostream>
// #include <iomanip>
// #include <algorithm>
// #include "chess.h"

#include <cstdint>
#include <immintrin.h>
#include <vector>
#include "chess.h"

namespace Talia
{

class BitBoard
{
    private:

        __m256i data; // 256-bit integer using AVX2
    
    public:

        // // Constructor: Default, initializes to all zeros
        // BitBoard() : data(_mm256_setzero_si256()) {}

        BitBoard(__m256i val) : data(val) {}

        BitBoard(uint64_t a = 0, uint64_t b = 0, uint64_t c = 0, uint64_t d = 0)
        {
            data = _mm256_set_epi64x(a, b, c, d);
        }

        ~BitBoard() = default;

        // Bitwise NOT
        BitBoard operator~() const {
            return BitBoard{_mm256_xor_si256(data, _mm256_set1_epi64x(-1))};
        }

        // Bitwise AND
        BitBoard operator&(const BitBoard& other) const {
            return BitBoard{_mm256_and_si256(data, other.data)};
        }

        // Bitwise OR
        BitBoard operator|(const BitBoard& other) const {
            return BitBoard{_mm256_or_si256(data, other.data)};
        }

        // Bitwise XOR
        BitBoard operator^(const BitBoard& other) const {
            return BitBoard{_mm256_xor_si256(data, other.data)};
        }

        // Logical AND
        bool operator&&(const BitBoard& other) const {
            return bool(*this) && bool(other);
        }

        // Logical OR
        bool operator||(const BitBoard& other) const {
            return bool(*this) || bool(other);
        }

        // Logical NOT
        bool operator!() const {
            return !_mm256_testz_si256(data, data);
        }

        // Assignment
        BitBoard& operator=(const BitBoard& other)
        {
            data = other.data;
            return *this;
        }

        // Compound AND
        BitBoard& operator&=(const BitBoard& other)
        {
            data = _mm256_and_si256(data, other.data);
            return *this;
        }

        // Compound OR
        BitBoard& operator|=(const BitBoard& other)
        {
            data = _mm256_or_si256(data, other.data);
            return *this;
        }

        // Compound XOR
        BitBoard& operator^=(const BitBoard& other)
        {
            data = _mm256_xor_si256(data, other.data);
            return *this;
        }

        BitBoard shiftLeft(int shift) const
        {
            if (shift <= 0) return *this;
            if (shift >= 256) return BitBoard{};
        
            const int byteShift = shift / 8;
            const int bitShift = shift % 8;
        
            __m256i bytes = _mm256_slli_si256(data, byteShift); // byte-wise shift
            __m256i carry = _mm256_srli_si256(data, 1);         // for cross-byte bit carry
            carry = _mm256_slli_epi64(carry, 64 - bitShift);    // prepare carry bits
            __m256i result = _mm256_or_si256(_mm256_slli_epi64(bytes, bitShift), carry);
            return BitBoard{result};
        }
        
        BitBoard shiftRight(int shift) const
        {
            if (shift <= 0) return *this;
            if (shift >= 256) return BitBoard{};
        
            const int byteShift = shift / 8;
            const int bitShift = shift % 8;
        
            __m256i bytes = _mm256_srli_si256(data, byteShift); // byte-wise shift
            __m256i carry = _mm256_slli_si256(data, 1);         // for cross-byte bit carry
            carry = _mm256_srli_epi64(carry, 64 - bitShift);    // prepare carry bits
            __m256i result = _mm256_or_si256(_mm256_srli_epi64(bytes, bitShift), carry);
            return BitBoard{result};
        }

        // Left shift (custom implementation)
        BitBoard operator<<(int shift) const {
            return shiftLeft(shift);
        }

        // Right shift (custom implementation)
        BitBoard operator>>(int shift) const {
            return shiftRight(shift);
        }

        // Compound Left shift
        BitBoard& operator<<=(int shift) {
            *this = shiftLeft(shift);
            return *this;
        }

        // Compound Right shift
        BitBoard& operator>>=(int shift) {
            *this = shiftRight(shift);
            return *this;
        }

        // Equality comparison: Returns true if both bitboards are identical
        inline bool operator==(const BitBoard& other) const noexcept {
            return _mm256_testc_si256(data, other.data) != 0;  // Test if the bitboards are the same
        }

        // Inequality comparison: Returns true if the bitboards are different
        inline bool operator!=(const BitBoard& other) const noexcept {
            return _mm256_testc_si256(data, other.data) == 0;  // Returns true if the bitboards differ
        }

        // Conversion to bool: Returns true if any bit is set (non-zero bitboard), otherwise false
        inline operator bool() const noexcept {
            return _mm256_testz_si256(data, data) == 0;  // Checks if any bit is set
        }

        // BitBoard byteswap() const {
        //     const __m256i mask = _mm256_set_epi8(
        //         0, 1, 2, 3, 4, 5, 6, 7,
        //         8, 9,10,11,12,13,14,15,
        //        16,17,18,19,20,21,22,23,
        //        24,25,26,27,28,29,30,31
        //     );
        //     __m256i reversed = _mm256_shuffle_epi8(
        //         _mm256_permute4x64_epi64(data, _MM_SHUFFLE(0,1,2,3)),
        //         _mm256_set_epi8(
        //             0, 1, 2, 3, 4, 5, 6, 7,
        //             8, 9,10,11,12,13,14,15,
        //            16,17,18,19,20,21,22,23,
        //            24,25,26,27,28,29,30,31
        //         )
        //     );
        //     return BitBoard{reversed};
        // }

        Square popLSB()
        {
            alignas(32) uint64_t arr[4];
            _mm256_store_si256((__m256i*)arr, data);
        
            for (int i = 0; i < 4; ++i) {
                if (arr[i]) {
                    uint64_t bit = arr[i] & -arr[i]; // isolate LSB
                    arr[i] ^= bit;                   // clear LSB
                    data = _mm256_load_si256((__m256i*)arr);
                    return Square(i * 64 + __builtin_ctzll(bit));
                }
            }
            return Square(255); // Invalid square
        }
        
        // Set a specific square (bit) to 1
        void setSQ(Square sq);

        // Pop a specific square (bit) (set it to 0)
        void popSQ(Square sq);

        // Set multiple squares (bits) at once
        void setSQ(std::vector<Square> squares);

        // Pop multiple squares (bits) at once
        void popSQ(std::vector<Square> squares);

        // Check if a specific square (bit) is set
        bool checkSQ(Square sq) const;

        // Display the BitBoard in a human-readable format
        void display() const noexcept;
};

inline uint64_t BB(int sq)
{
    return 1ULL << sq;
}

inline uint64_t BB(const std::vector<int>& squares)
{
    uint64_t bb = 0;

    for (auto sq: squares)
    {
        bb |= 1ULL << sq;
    }

    return bb;
}

//       inline bool isEdgeOrCorner(int row, int col) noexcept
// {
//     // Check if the square is on the edge
//     if (row == 0 || row == 15 || col == 0 || col == 15)
//         return true;

//     // Check if the square is in any of the 5x5 corners
//     if ((row < 5 && col < 5) || // Top-left corner
//         (row < 5 && col > 10) || // Top-right corner
//         (row > 10 && col < 5) || // Bottom-left corner
//         (row > 10 && col > 10))  // Bottom-right corner
//         return true;

//     return false;
// }

// // 256 bit unsgined integer
// class BitBoard
// {
//     private:

//         __uint128_t high;  // Most  significant 128 bits
//         __uint128_t low;   // Least significant 128 bits

//     public:

//         BitBoard() noexcept : high(0), low(0) {}

//         BitBoard(__uint128_t h, __uint128_t l) noexcept : high(h), low(l) {}

//         // BitBoard(Square sq) noexcept : high(0), low(0) {}

//         BitBoard(Square sq) noexcept : high(0), low(0)
//         {
//             // Assuming the `encoded` field in Square represents the position (0 to 255)
//             // uint8_t position = sq.getEncoded();  // Access the encoded value
    
//             if (sq < 128) {
//                 // If the position is within the lower 128 bits
//                 low = (__uint128_t(1) << sq);
//             } else {
//                 // If the position is within the higher 128 bits
//                 high = (__uint128_t(1) << (sq - 128));
//             }
//         }

//         ~BitBoard() noexcept = default;

// //         // Copy constructor and copy assignment operator (defaulted)
// //         BitBoard(const BitBoard&) noexcept = default;
// //         BitBoard& operator=(const BitBoard&) noexcept = default;

// //         // Explicitly default move constructor and move assignment operator with noexcept.
// //         BitBoard(BitBoard&&) noexcept = default;
// //         BitBoard& operator=(BitBoard&&) noexcept = default;

//         // -------------------------------
        
//         BitBoard operator&(const BitBoard& other) const noexcept {
//             return BitBoard(high & other.high, low & other.low);
//         }

//         BitBoard operator|(const BitBoard& other) const noexcept {
//             return BitBoard(high | other.high, low | other.low);
//         }

//         BitBoard operator^(const BitBoard& other) const noexcept {
//             return BitBoard(high ^ other.high, low ^ other.low);
//         }

//         BitBoard operator~() const noexcept {
//             return BitBoard(~high, ~low);
//         }


//         // BitBoard operator<<(int shift) const noexcept {
//         //     if (shift == 0) {
//         //         // No shift
//         //         return *this;
//         //     } else if (shift < 128) {
//         //         // Shift within the 128-bit range
//         //         __uint128_t new_high = (high << shift) | (low >> (128 - shift));
//         //         __uint128_t new_low = low << shift;
//         //         return BitBoard(new_high, new_low);
//         //     } else if (shift < 256) {
//         //         // Shift entirely from low into high
//         //         __uint128_t new_high = low << (shift - 128);
//         //         return BitBoard(new_high, 0); // Low becomes zero
//         //     } else {
//         //         // Shift beyond 256 bits results in zero
//         //         return BitBoard(0, 0);
//         //     }
//         // }

//         // BitBoard operator>>(int shift) const noexcept {
//         //     if (shift == 0) {
//         //         // No shift
//         //         return *this;
//         //     } else if (shift < 128) {
//         //         // Shift within the 128-bit range
//         //         __uint128_t new_low = (low >> shift) | (high << (128 - shift));
//         //         __uint128_t new_high = high >> shift;
//         //         return BitBoard(new_high, new_low);
//         //     } else if (shift < 256) {
//         //         // Shift entirely from high into low
//         //         __uint128_t new_low = high >> (shift - 128);
//         //         return BitBoard(0, new_low); // High becomes zero
//         //     } else {
//         //         // Shift beyond 256 bits results in zero
//         //         return BitBoard(0, 0);
//         //     }
//         // }

//         BitBoard operator<<(int shift) const noexcept
//         {
//             // Handle shifts naturally without branching
//             __uint128_t new_high = (high << shift) | (low >> (128 - shift));
//             __uint128_t new_low = low << shift;

//             // Handle the case where shift >= 128 implicitly
//             if (shift >= 128) {
//                 new_high = low << (shift - 128);  // Shift low into high
//                 new_low = 0;                      // Low becomes zero
//             }

//             return BitBoard(new_high, new_low);
//         }

//         // Right shift operator: simplified
//         BitBoard operator>>(int shift) const noexcept
//         {
//             return shift < 128 
//                 ? BitBoard(high >> shift, (low >> shift) | (high << (128 - shift)))
//                 : BitBoard(0, high >> (shift - 128));

//             // // Handle shifts naturally without branching
//             // __uint128_t new_low = (low >> shift) | (high << (128 - shift));
//             // __uint128_t new_high = high >> shift;

//             // // Handle the case where shift >= 128 implicitly
//             // if (shift >= 128) {
//             //     new_low = high >> (shift - 128);  // Shift high into low
//             //     new_high = 0;                     // High becomes zero
//             // }

//             // return BitBoard(new_high, new_low);
//         }

//         bool operator==(const BitBoard& other) const noexcept {
//             return (high == other.high) && (low == other.low);
//         }

//         bool operator!=(const BitBoard& other) const noexcept {
//             return !(*this == other);
//         }

//         // -------------------------------

//         BitBoard& operator&=(const BitBoard& other) noexcept
//         {
//             high &= other.high;
//             low  &= other.low;
//             return *this;
//         }

//         BitBoard& operator|=(const BitBoard& other) noexcept
//         {
//             high |= other.high;
//             low  |= other.low;
//             return *this;
//         }

//         BitBoard& operator^=(const BitBoard& other) noexcept
//         {
//             high ^= other.high;
//             low  ^= other.low;
//             return *this;
//         }

//         // -------------------------------

//         void setSQ(Square sq) noexcept
//         {
//             (sq < 128 ? low : high) |= (__uint128_t(1) << (sq & 127));
//         }

//         void popSQ(Square sq) noexcept
//         {
//             (sq < 128 ? low : high) &= ~(__uint128_t(1) << (sq & 127));
//         }

//         [[nodiscard]] Square getLSB() const noexcept
//         {
//             if      (low  != 0) return __builtin_ctzll(low);
//             else if (high != 0) return __builtin_ctzll(high) + 128;

//             return OFFBOARD;
//         }

//         // Square getMSB() const noexcept
//         // {
//         //     if (high != 0) {
//         //         return 128 + (127 - __builtin_clzll(static_cast<unsigned long long>(high)));
//         //     } else if (low != 0) {
//         //         return 127 - __builtin_clzll(static_cast<unsigned long long>(low));
//         //     }
//         //     return OFFBOARD; // No bits are set
//         // }

//         // int count() const noexcept
//         // {
//         //     return __builtin_popcountll(static_cast<unsigned long long>(low)) +
//         //            __builtin_popcountll(static_cast<unsigned long long>(high));
//         // }

//         // Square popLSB() noexcept {
//         //     if (low != 0) {
//         //         Square index = __builtin_ctzll(static_cast<unsigned long long>(low));
//         //         low &= low - 1; // Clear the least significant bit
//         //         return index;
//         //     } else if (high != 0) {
//         //         Square index = 128 + __builtin_ctzll(static_cast<unsigned long long>(high));
//         //         high &= high - 1; // Clear the least significant bit
//         //         return index;
//         //     }
//         //     return OFFBOARD; // No bits are set
//         // }
    
//         // // Pop the most significant bit (MSB) and return its index (efficient version)
//         // Square popMSB() noexcept {
//         //     if (high != 0) {
//         //         Square index = 128 + (127 - __builtin_clzll(static_cast<unsigned long long>(high)));
//         //         high &= ~(__uint128_t(1) << (index - 128));
//         //         return index;
//         //     } else if (low != 0) {
//         //         Square index = (127 - __builtin_clzll(static_cast<unsigned long long>(low)));
//         //         low &= ~(__uint128_t(1) << index);
//         //         return index;
//         //     }
//         //     return OFFBOARD; // No bits are set
//         // }

//         [[nodiscard]] bool checkSQ(Square sq) const noexcept
//         {
//             return ((BitBoard(0, 1) << sq) & *this);

//             // if (sq < 128) 
//             //      return (low  & (__uint128_t(1) << sq))         != 0;
//             // else return (high & (__uint128_t(1) << (sq - 128))) != 0;


//         }

//         // -------------------------------

//         [[nodiscard]] operator bool() const noexcept { return high || low; }

//         // -------------------------------

  

// void display() const noexcept
// {
//     std::cout << "\n";
//     for (int row = 15; row >= 0; --row)
//     {
//         std::string line;
//         for (int col = 0; col < 16; ++col)
//         {
//             int index = row * 16 + col;

//             if (checkSQ(index))
//             {
//                 // Color occupied squares (X) blue
//                 line += "\033[35mX\033[0m";
//             }
//             else if (isEdgeOrCorner(row, col))
//             {
//                 // Color edge or corner squares darker gray
//                 line += "\033[90m.\033[0m"; // Gray for edge/corner squares
//             }
//             else
//             {
//                 // Default color for empty squares
//                 line.push_back('.');
//             }

//             if (col < 15) line.push_back(' ');
//         }
//         std::cout << line << "\n";
//     }
//     std::cout << "\n";
// }

// //         void display() const noexcept
// // {
// //     std::cout << "\n";
// //     for (int row = 15; row >= 0; --row)
// //     {
// //         std::string line;
// //         for (int col = 0; col < 16; ++col)
// //         {
// //             int index = row * 16 + col;

// //             if (checkSQ(index))
// //             {
// //                 // Add blue color for 'X'
// //                 line += "\033[34mX\033[0m"; // "\033[34m" sets the color to blue, "\033[0m" resets it
// //             }
// //             else
// //             {
// //                 line.push_back('.');
// //             }

// //             if (col < 15) line.push_back(' ');
// //         }
// //         std::cout << line << "\n";
// //     }
// //     std::cout << "\n";
// // }

//         // void display() const noexcept
//         // {
//         //     std::cout << "\n";
//         //     for (int row = 15; row >= 0; --row)
//         //     {
//         //         std::string line;
//         //         for (int col = 0; col < 16; ++col)
//         //         {
//         //             int index = row * 16 + col;
//         //             line.push_back(checkSQ(index) ? 'X' : '.');
//         //             if (col < 15) line.push_back(' ');
//         //         }
//         //         std::cout << line << "\n";
//         //     }
//         //     std::cout << "\n";
//         // }
// };

// using u128 = __uint128_t;
// using u256 = BitBoard;

// // shift
// // mask
// // attack

} // namespace Talia

#endif