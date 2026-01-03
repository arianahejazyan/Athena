#include <iostream>
#include "bitboard.h"

namespace athena
{

void Bitboard::print(bool debug) const
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
            auto sq = buildSquare(r, f);
            bool valid = isStone(sq);
            bool set = hasSquare(sq);
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