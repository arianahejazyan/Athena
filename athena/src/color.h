#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include "constants.h"  

namespace athena
{

// Forward declaration
class Color;

class Team
{
    public: enum : uint8_t
    {
        RY = 0,
        BG = 1,
    };

    public:
    
    // Default constructor
    constexpr Team() = default;

    // Constructor
    constexpr Team(uint8_t enum_) noexcept : enum_(enum_) {}

    // Conversion operator
    explicit constexpr operator uint8_t() const noexcept { return enum_; }

    constexpr Team enemy() const noexcept { return Team(enum_ ^ 1); }

    constexpr Color rb() const noexcept;
    constexpr Color yg() const noexcept;

    public:
    uint8_t enum_;
};



// class Alliance
// {
//     public: enum : uint8_t
//     {
//         RY = 0,
//         BG = 1,
//     };

//     private:
//     uint8_t value_;
// };

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

    constexpr Team team()  const noexcept { return Team( value_ & 0b1       ); }
    constexpr Team enemy() const noexcept { return Team((value_ & 0b1) ^ 0b1); }

    // Return the UCI representation of the color
    std::string uci() const noexcept;

    // Return true if the two colors are teammates
    constexpr bool is_teammate(Color color) noexcept { return ((value_ ^ color.value_) & 0b1) == 0b0; }

    // Return true if the two colors are opponents
    constexpr bool is_opponent(Color color) noexcept { return ((value_ ^ color.value_) & 0b1) == 0b1; }

    // private:
    uint8_t value_;
};

constexpr Color Team::rb() const noexcept { return Color(enum_       ); }
constexpr Color Team::yg() const noexcept { return Color(enum_ | 0b10); }

inline constexpr bool operator==(Team lhs, Team rhs) noexcept {
    return static_cast<uint8_t>(lhs) == static_cast<uint8_t>(rhs);
}

inline constexpr bool operator!=(Team lhs, Team rhs) noexcept {
    return static_cast<uint8_t>(lhs) != static_cast<uint8_t>(rhs);
}

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

consteval std::array<Team, TEAM_NB> team_array()
{
    // std::array<Team, TEAM_NB> arr = {Team::RY, Team::BG};
    return {Team::RY, Team::BG};
}

} // namespace athena