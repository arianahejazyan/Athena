#include "bitboard.h"

namespace Talia
{
    
[[nodiscard]] bool checkbb(BitBoard bb, Square sq)
{
    assert(A1 <= sq && sq <= H8);

    return bb & BB(sq);
}

void printbb(BitBoard bb)
{
    for (int rank = 7; rank >= 0; rank--)
    {
        std::string row = ". . . . . . . .";

        for (int file = 0; file < FILE_NB; file++)
            if (checkbb(bb, makeSquare(rank, file))) row[2 * file] = 'X';

        std::cout << row << std::endl;        
    }

    std::cout << std::endl; 
}

} // namespace
