#include <immintrin.h>
#include "bitboards.h"

namespace ELSA
{
    
Square makeSquare(u32 rank, u32 file) {
    return (Square)(rank * FILE_NB + file); 
}

u64 BB(Square sq) {
    return 1ull << sq; 
}

u32 getRank(u32 sq) {
    return sq >> 0x3; 
}

u32 getFile(u32 sq) { 
    return sq & 0x7; 
}

}; // namespace