#include "castle.h"

namespace athena
{

Castle::Castle(std::string_view castle, Side side) noexcept : value_(0)
{
    if (castle[0] == '1') value_ |= Castle(Color::Red   , side).value_; else
    if (castle[2] == '1') value_ |= Castle(Color::Blue  , side).value_; else
    if (castle[4] == '1') value_ |= Castle(Color::Yellow, side).value_; else
    if (castle[6] == '1') value_ |= Castle(Color::Green , side).value_;
}

Castle::Castle(std::string_view kingside, std::string_view queenside) noexcept : value_(0)
{
    value_ |= Castle( kingside, Side::KingSide ).value_;
    value_ |= Castle(queenside, Side::QueenSide).value_;
}

std::string Castle::uci(Side side) const noexcept
{
    std::string output = "";
    output += ((value_ & Castle(Color::Red   , side).value_) ? '1' : '0'); output += ',';
    output += ((value_ & Castle(Color::Blue  , side).value_) ? '1' : '0'); output += ',';
    output += ((value_ & Castle(Color::Yellow, side).value_) ? '1' : '0'); output += ',';
    output += ((value_ & Castle(Color::Green , side).value_) ? '1' : '0');
    return output;
}

} // namespace athena