#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include "constants.h"  

namespace athena
{

class Color
{
    public: enum : uint8_t
    {
        Red    = 0,
        Blue   = 1,
        Yellow = 2,
        Green  = 3,
        RY     = 4,
        BG     = 5,
        RBYG   = 6,
        None   = 7,
    };

    public:

    // Default constructor
    constexpr Color() = default;

    // Constructor
    constexpr Color(uint8_t value) noexcept : value_(value) {}

    // UCI Constructor
    Color(std::string_view color) noexcept;

    // Conversion operator
    explicit constexpr operator uint8_t() const noexcept { return value_; }

    // Return the next color
    constexpr Color next() const noexcept { return Color((value_ + 1) & 0b011); }

    // Return the ally color
    constexpr Color ally() const noexcept { return Color((value_ + 2) & 0b011); }

    // Return the previous color
    constexpr Color prev() const noexcept { return Color((value_ + 3) & 0b011); }

    // Return the UCI representation of the color
    std::string uci() const noexcept;

    // Return true if the two colors are teammates
    constexpr bool is_teammate(Color color) noexcept { return ((value_ ^ color.value_) & 0b1) == 0b0; }

    // Return true if the two colors are opponents
    constexpr bool is_opponent(Color color) noexcept { return ((value_ ^ color.value_) & 0b1) == 0b1; }

    private:
    uint8_t value_;
};

// ===== operators ===== //

inline constexpr bool operator==(Color lhs, Color rhs) noexcept {
    return static_cast<uint8_t>(lhs) == static_cast<uint8_t>(rhs);
}

inline constexpr bool operator!=(Color lhs, Color rhs) noexcept {
    return static_cast<uint8_t>(lhs) != static_cast<uint8_t>(rhs);
}

// ===== precomputed tables ===== //

inline constexpr std::array<std::array<Color, GUIDES_SIZE>, COLOR_NB> GUILDS = 
{{
    {Color::Red, Color::Yellow},
    {Color::Blue, Color::Green},
    {Color::Yellow, Color::Red},
    {Color::Green, Color::Blue},
}};

inline constexpr std::array<Color, GUIDES_SIZE> opponents(Color color) noexcept {
    return GUILDS[static_cast<uint8_t>(color.next())];
}

inline constexpr std::array<Color, GUIDES_SIZE> teammates(Color color) noexcept {
    return GUILDS[static_cast<uint8_t>(color)];
}

} // namespace athena