#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <immintrin.h>
#include "chess.hpp"
#include "types.hpp"
namespace ELSA
{
    Square makeSquare(u32 rank, u32 file) {
        return static_cast<Square>(rank * FILE_SIZE + file); 
    }

}; // namespace

#endif