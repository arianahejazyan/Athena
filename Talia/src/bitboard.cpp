#include <iostream>
#include "bitboard.h"

namespace Talia
{

bool BitBoard::checkSQ(Square sq) const
{
    // Determine the lane (0, 1, 2, 3) based on the square index
    int lane = sq >> 6;   // sq / 64 -> Determines the lane (0, 1, 2, 3)
    int bit  = sq & 0x3F; // sq % 64 -> Determines the bit position within the lane

    // Extract the 64-bit lane from the 256-bit __m256i data using _mm256_extract_epi64
    // Use the appropriate lane index directly as an integer constant

    uint64_t value;
    switch (lane) {
        case 0:
            value = _mm256_extract_epi64(data, 0);  // Extract the first 64 bits
            break;
        case 1:
            value = _mm256_extract_epi64(data, 1);  // Extract the second 64 bits
            break;
        case 2:
            value = _mm256_extract_epi64(data, 2);  // Extract the third 64 bits
            break;
        case 3:
            value = _mm256_extract_epi64(data, 3);  // Extract the fourth 64 bits
            break;
        default:
            return false;  // Invalid lane index
    }

    // Check if the specific bit is set in the extracted 64-bit value
    return (value & (1ULL << bit)) != 0;
}

void BitBoard::setSQ(Square sq)
{
    int lane = sq >> 6;   // sq / 64 -> Determines which lane (0, 1, 2, 3) the bit is in
    int bit  = sq & 0x3F; // sq % 64 -> Determines the bit position within the lane

    // Create a 64-bit mask with the target bit set to 1
    uint64_t mask = 1ULL << bit;

    // Create a 256-bit mask with the target bit set in the correct lane
    __m256i mask256 = _mm256_set_epi64x(
        lane == 3 ? mask : 0,  // Lane 3 (highest) gets the mask in the correct bit
        lane == 2 ? mask : 0,  // Lane 2 gets the mask in the correct bit
        lane == 1 ? mask : 0,  // Lane 1 gets the mask in the correct bit
        lane == 0 ? mask : 0   // Lane 0 gets the mask in the correct bit
    );

    // Set the corresponding bit in the data
    data = _mm256_or_si256(data, mask256);
}

void BitBoard::popSQ(Square sq)
{
    int lane = sq >> 6;   // sq / 64 -> Determines which lane (0, 1, 2, 3) the bit is in
    int bit  = sq & 0x3F; // sq % 64 -> Determines the bit position within the lane

    // Create a 64-bit mask with the target bit set to 1
    uint64_t mask = 1ULL << bit;

    // Create a 256-bit mask with the target bit set in the correct lane
    __m256i mask256 = _mm256_set_epi64x(
        lane == 3 ? mask : 0,  // Lane 3 (highest) gets the mask in the correct bit
        lane == 2 ? mask : 0,  // Lane 2 gets the mask in the correct bit
        lane == 1 ? mask : 0,  // Lane 1 gets the mask in the correct bit
        lane == 0 ? mask : 0   // Lane 0 gets the mask in the correct bit
    );

    // Set the corresponding bit to 0 by AND-ing with the negation of the mask
    data = _mm256_andnot_si256(mask256, data);
}

void BitBoard::setSQ(std::vector<Square> squares)
{
    for (const auto& sq : squares) {
        setSQ(sq);
    }
}

void BitBoard::popSQ(std::vector<Square> squares)
{
    for (const auto& sq : squares) {
        popSQ(sq);
    }
}

void BitBoard::display() const noexcept
{
    std::cout << "\n";
    for (int row = 15; row >= 0; --row)
    {
        std::string line;
        for (int col = 0; col < 16; ++col)
        {
            int index = row * 16 + col;
            Square sq(index);  // Create a Square instance from the index

            if (checkSQ(index))
            {
                // Color occupied squares (X) blue
                line += "\033[35mX\033[0m";
            }
            else if (sq.isEdgeOrCorner())  // Use the Square instance to check edge/corner
            {
                // Color edge or corner squares darker gray
                line += "\033[90m.\033[0m"; // Gray for edge/corner squares
            }
            else
            {
                // Default color for empty squares
                line.push_back('.');
            }

            if (col < 15) line.push_back(' ');
        }
        std::cout << line << "\n";
    }
    std::cout << "\n";
}

} // namespace Talia