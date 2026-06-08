#include "square.h"

namespace athena
{

Square::Square(std::string_view square, bool expanded) noexcept
{
    int r = std::stoi(std::string(square.substr(1))); 
    int f = square[0] - 'a' + 1;

    if (expanded)
    {
        r--;
        f--;
    }

    value_ = r * FILE_NB + f;
}

std::string Square::uci(bool expanded) const noexcept
{
    if (offboard()) return "-";

    int r = rank();
    int f = file() - 1;

    if (expanded)
    {
        r++;
        f++;
    }

    return std::string(1, static_cast<char>('a' + f)) + std::to_string(r);
}

} // namespace athena