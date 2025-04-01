#include "hyperbola_quintessence.h"

namespace Talia
{

consteval static BitBoard slide(const Square sq, const int delta_x, const int delta_y)
{
    BitBoard mask = 0ULL;

    int r = rankSQ(sq) + delta_x;
    int f = fileSQ(sq) + delta_y;

    while (isValidSquare(r, f))
    {
        setbb(mask, makeSquare(r, f));

        r += delta_x;
        f += delta_y;
    }

    return mask;
}

constexpr StaticArray<BitBoard, SQUARE_NB> DIAGONAL_MASK = []() consteval
{
    StaticArray<BitBoard, SQUARE_NB> arr{};

    for (int s = A1; s <= H8; ++s)
    {
        Square sq = static_cast<Square>(s);

        setbb(arr[sq], slide(sq, +1, +1));
        setbb(arr[sq], slide(sq, -1, -1));
    }

    return arr;
}();

constexpr StaticArray<BitBoard, SQUARE_NB> ANTIDIAGONAL_MASK = []() consteval
{
    StaticArray<BitBoard, SQUARE_NB> arr{};

    for (int s = A1; s <= H8; ++s)
    {
        Square sq = static_cast<Square>(s);

        setbb(arr[sq], slide(sq, +1, -1));
        setbb(arr[sq], slide(sq, -1, +1));
    }

    return arr;
}();

constexpr StaticArray<BitBoard, SQUARE_NB> VERTICAL_MASK = []() consteval
{
    StaticArray<BitBoard, SQUARE_NB> arr{};

    for (int s = A1; s <= H8; ++s)
    {
        Square sq = static_cast<Square>(s);

        setbb(arr[sq], slide(sq, +1, 0));
        setbb(arr[sq], slide(sq, -1, 0));
    }

    return arr;
}();

constexpr StaticArray<BitBoard, FILE_NB * 64> HORIZONTAL_MASK = []() consteval
{
    StaticArray<BitBoard, FILE_NB * 64> arr{};

    for (int x = 0; x < 64; ++x) {
        for (int f = 0; f < 8; ++f) {
            int o = 2 * x;
            int x2{}, y2{};
            int b{};

            y2 = 0;
            for (x2 = f - 1; x2 >= 0; --x2) {
                b = 1 << x2;
                y2 |= b;
                if ((o & b) == b) break;
            }
            for (x2 = f + 1; x2 < 8; ++x2) {
                b = 1 << x2;
                y2 |= b;
                if ((o & b) == b) break;
            }
            arr[x * 8ull + f] = y2;
        }
    }

    return arr;
}();

} // namespace