#pragma once

#include <array>
#include "color.h"
#include "square.h"
#include "constants.h"

namespace athena
{

enum class Side : uint8_t
{
    KingSide  = 0,
    QueenSide = 1,
};

class Castle
{
    public:

    // Default constructor
    constexpr Castle() = default;

    // Constructor
    constexpr Castle(uint8_t value) noexcept : value_(value) {}

    // Constructor
    constexpr Castle(Side side) noexcept 
        : value_(0b01010101 << static_cast<uint8_t>(side)) {}

    // Constructor
    constexpr Castle(Color color) noexcept 
        : value_(0b11 << (static_cast<uint8_t>(color) << 1)) {}

    // Constructor
    constexpr Castle(Color color, Side side) noexcept
        : value_(1 << ((static_cast<uint8_t>(color) << 1) + static_cast<uint8_t>(side))) {}

    // UCI Constructor
    Castle(std::string_view castle, Side side) noexcept;

    // UCI Constructor
    Castle(std::string_view kingside, std::string_view queenside) noexcept;

    // Conversion operator
    explicit constexpr operator uint8_t() const noexcept {
        return value_;
    }

    // Check if the castle is empty
    constexpr bool empty() const noexcept {
        return value_ == 0;
    }

    // UCI Conversion
    std::string uci(Side side) const noexcept;

    private:
    uint8_t value_;
};

// enum class Castle : uint8_t
// {
//     rKS = 1 << ((static_cast<uint8_t>(Color:: Red   ) * 2) + static_cast<uint8_t>(Side::KingSide )), // 0b00000001
//     rQS = 1 << ((static_cast<uint8_t>(Color:: Red   ) * 2) + static_cast<uint8_t>(Side::QueenSide)),
//     bKS = 1 << ((static_cast<uint8_t>(Color:: Blue  ) * 2) + static_cast<uint8_t>(Side::KingSide )),
//     bQS = 1 << ((static_cast<uint8_t>(Color:: Blue  ) * 2) + static_cast<uint8_t>(Side::QueenSide)),
//     yKS = 1 << ((static_cast<uint8_t>(Color:: Yellow) * 2) + static_cast<uint8_t>(Side::KingSide )),
//     yQS = 1 << ((static_cast<uint8_t>(Color:: Yellow) * 2) + static_cast<uint8_t>(Side::QueenSide)),
//     gKS = 1 << ((static_cast<uint8_t>(Color:: Green ) * 2) + static_cast<uint8_t>(Side::KingSide )),
//     gQS = 1 << ((static_cast<uint8_t>(Color:: Green ) * 2) + static_cast<uint8_t>(Side::QueenSide)),
// };

// consteval Castle buildCastle(Color color, Side side) noexcept {
//     return static_cast<Castle>(1 << ((2 * static_cast<uint8_t>(color)) + static_cast<uint8_t>(side)));
// }

// template<uint8_t color, Side side>
// bool hasCastle(Castle castle) noexcept {
//     return (static_cast<uint8_t>(castle) & static_cast<uint8_t>(buildCastle(color, side))) != 0;
// }

consteval std::array<Square, 4> castle_squares(Color color, Side side)
{
    #ifdef GAME_SETUP_MODERN
    std::array<std::array<Square, 4>, COLOR_NB> KINGSIDE_SQUARES = 
    {{
        {Square::I2 , Square::K2 , Square::L2 , Square::J2 },
        {Square::B8 , Square::B6 , Square::B5 , Square::B7 }, 
        {Square::H15, Square::F15, Square::E15, Square::G15},  
        {Square::O9 , Square::O11, Square::O12, Square::O10}, 
    }};

    std::array<std::array<Square, 4>, COLOR_NB> QUEENSIDE_SQUARES = 
    {{
        {Square::I2 , Square::G2 , Square::E2 , Square::H2 },
        {Square::B8 , Square::B10, Square::B12, Square::B9 },
        {Square::H15, Square::J15, Square::L15, Square::I15},
        {Square::O9 , Square::O7 , Square::O5 , Square::O8 },
    }};
    #else
    std::array<std::array<Square, 4>, COLOR_NB> KINGSIDE_SQUARES = 
    {{
        {Square::I2 , Square::K2 , Square::L2 , Square::J2 },
        {Square::B9 , Square::B11, Square::B12, Square::B10},
        {Square::H15, Square::F15, Square::E15, Square::G15}, 
        {Square::O8 , Square::O6 , Square::O5 , Square::O7 }, 
    }};

    std::array<std::array<Square, 4>, COLOR_NB> QUEENSIDE_SQUARES = 
    {{
        {Square::I2 , Square::G2 , Square::E2 , Square::H2 },
        {Square::B9 , Square::B7 , Square::B5 , Square::B8 },
        {Square::H15, Square::J15, Square::L15, Square::I15},
        {Square::O8 , Square::O10, Square::O12, Square::O9 },
    }};
    #endif

    return 
    side == Side::KingSide 
    ?KINGSIDE_SQUARES[static_cast<uint8_t>(color)]: 
    QUEENSIDE_SQUARES[static_cast<uint8_t>(color)];
}

consteval std::array<Square, 3> path_squares(Color color, Side side)
{
    #ifdef GAME_SETUP_MODERN
    std::array<std::array<Square, 3>, COLOR_NB> KINGSIDE_SQUARES = 
    {{
        {Square::J2 , Square::K2 , Square::K2 },
        {Square::B6 , Square::B7 , Square::B7 }, 
        {Square::F15, Square::G15, Square::G15},  
        {Square::O10, Square::O11, Square::O11}, 
    }};

    std::array<std::array<Square, 3>, COLOR_NB> QUEENSIDE_SQUARES = 
    {{
        {Square::F2 , Square::G2 , Square::H2 },
        {Square::B9 , Square::B10, Square::B11},
        {Square::I15, Square::J15, Square::K15},
        {Square::O6 , Square::O7 , Square::O8 },
    }};
    #else
    std::array<std::array<Square, 3>, COLOR_NB> KINGSIDE_SQUARES = 
    {{
        {Square::J2 , Square::K2 , Square::K2 },
        {Square::B10, Square::B11, Square::B11},
        {Square::F15, Square::G15, Square::G15}, 
        {Square::O6 , Square::O7 , Square::O7 }, 
    }};

    std::array<std::array<Square, 3>, COLOR_NB> QUEENSIDE_SQUARES = 
    {{
        {Square::F2 , Square::G2 , Square::H2 },
        {Square::B6 , Square::B7 , Square::B8 },
        {Square::I15, Square::J15, Square::K15},
        {Square::O9 , Square::O10, Square::O11},
    }};
    #endif

    return 
    side == Side::KingSide 
    ?KINGSIDE_SQUARES[static_cast<uint8_t>(color)]: 
    QUEENSIDE_SQUARES[static_cast<uint8_t>(color)];
}

consteval std::array<Square, 2> safe_squares(Color color, Side side)
{
    #ifdef GAME_SETUP_MODERN
    std::array<std::array<Square, 2>, COLOR_NB> KINGSIDE_SQUARES = 
    {{
        {Square::I2 , Square::J2 },
        {Square::B7 , Square::B8 }, 
        {Square::G15, Square::H15}, 
        {Square::O9 , Square::O10}, 
    }};

    std::array<std::array<Square, 2>, COLOR_NB> QUEENSIDE_SQUARES = 
    {{
        {Square::H2 , Square::I2 },
        {Square::B8 , Square::B9 },
        {Square::H15, Square::I15},
        {Square::O8 , Square::O9 },
    }};
    #else
    std::array<std::array<Square, 2>, COLOR_NB> KINGSIDE_SQUARES = 
    {{
        {Square::I2 , Square::J2 },
        {Square::B9 , Square::B10},
        {Square::G15, Square::H15},
        {Square::O7 , Square::O8 }, 
    }};

    std::array<std::array<Square, 2>, COLOR_NB> QUEENSIDE_SQUARES = 
    {{
        {Square::H2 , Square::I2 },
        {Square::B8 , Square::B9 },
        {Square::H15, Square::I15},
        {Square::O8 , Square::O9 },
    }};
    #endif

    return 
    side == Side::KingSide
    ?KINGSIDE_SQUARES[static_cast<uint8_t>(color)]: 
    QUEENSIDE_SQUARES[static_cast<uint8_t>(color)];
}


// struct CastleSquares
// {
//     Square king_source;
//     Square king_target;
//     Square rook_source;
//     Square rook_target;
//     std::array<Square, 3> path;
//     std::array<Square, 2> safe;
// };

// inline constexpr std::array<std::array<std::array<CastleSquares, SIDE_NB>, COLOR_NB>, SETUP_NB> CASTLE_SQUARES = 
// {{
// {{
//     {{
//         {Square::I2 , Square::K2 , Square::L2 , Square::J2 , {Square::J2 , Square::K2 , Square::K2 }, {Square::I2 , Square::J2 }}, 
//         {Square::I2 , Square::G2 , Square::E2 , Square::H2 , {Square::F2 , Square::G2 , Square::H2 }, {Square::H2 , Square::I2 }},
//         }}, 
//     {{
//         {Square::B8 , Square::B6 , Square::B5 , Square::B7 , {Square::B6 , Square::B7 , Square::B7 }, {Square::B7 , Square::B8 }}, 
//         {Square::B8 , Square::B10, Square::B12, Square::B9 , {Square::B9 , Square::B10, Square::B11}, {Square::B8 , Square::B9 }},
//         }}, 
//     {{
//         {Square::H15, Square::F15, Square::E15, Square::G15, {Square::F15, Square::G15, Square::G15}, {Square::G15, Square::H15}}, 
//         {Square::H15, Square::J15, Square::L15, Square::I15, {Square::I15, Square::J15, Square::K15}, {Square::H15, Square::I15}},
//         }}, 
//     {{
//         {Square::O9 , Square::O11, Square::O12, Square::O10, {Square::O10, Square::O11, Square::O11}, {Square::O9 , Square::O10}}, 
//         {Square::O9 , Square::O7 , Square::O5 , Square::O8 , {Square::O6 , Square::O7 , Square::O8 }, {Square::O8 , Square::O9 }},
//         }}
//     }},
// {{
//     {{
//         {Square::I2 , Square::K2 , Square::L2 , Square::J2 , {Square::J2 , Square::K2 , Square::K2 }, {Square::I2 , Square::J2 }}, 
//         {Square::I2 , Square::G2 , Square::E2 , Square::H2 , {Square::F2 , Square::G2 , Square::H2 }, {Square::H2 , Square::I2 }},
//         }}, 
//     {{
//         {Square::B9 , Square::B11, Square::B12, Square::B10, {Square::B10, Square::B11, Square::B11}, {Square::B9 , Square::B10}}, 
//         {Square::B9 , Square::B7 , Square::B5 , Square::B8 , {Square::B6 , Square::B7 , Square::B8 }, {Square::B8 , Square::B9 }},
//         }}, 
//     {{
//         {Square::H15, Square::F15, Square::E15, Square::G15, {Square::F15, Square::G15, Square::G15}, {Square::G15, Square::H15}}, 
//         {Square::H15, Square::J15, Square::L15, Square::I15, {Square::I15, Square::J15, Square::K15}, {Square::H15, Square::I15}},
//         }}, 
//     {{
//         {Square::O8 , Square::O6 , Square::O5 , Square::O7 , {Square::O6 , Square::O7 , Square::O7 }, {Square::O7 , Square::O8 }}, 
//         {Square::O8 , Square::O10, Square::O12, Square::O9 , {Square::O9 , Square::O10, Square::O11}, {Square::O8 , Square::O9 }},
//         }}
//     }}
// }};

// ===== operators ===== //

// Bitwise and operator
constexpr Castle operator&(Castle lhs, Castle rhs) noexcept {
    return static_cast<Castle>(
        static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs)
    );
}

// Bitwise xor operator
constexpr Castle operator^(Castle lhs, Castle rhs) noexcept {
    return static_cast<Castle>(
        static_cast<uint8_t>(lhs) ^ static_cast<uint8_t>(rhs)
    );
}

// Bitwise or operator
constexpr Castle operator|(Castle lhs, Castle rhs) noexcept {
    return static_cast<Castle>(
        static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs)
    );
}

// Bitwise left shift operator
constexpr Castle operator<<(Castle lhs, uint8_t shift) noexcept {
    return static_cast<Castle>(
        static_cast<uint8_t>(lhs) << shift
    );
}

// Bitwise right shift operator
constexpr Castle operator>>(Castle lhs, uint8_t shift) noexcept {
    return static_cast<Castle>(
        static_cast<uint8_t>(lhs) >> shift
    );
}

// Bitwise and assignment operator
constexpr Castle& operator&=(Castle& lhs, Castle rhs) noexcept {
    lhs = static_cast<Castle>(
        static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs)
    );
    return lhs;
}

// Bitwise xor assignment operator
constexpr Castle& operator^=(Castle& lhs, Castle rhs) noexcept {
    lhs = static_cast<Castle>(
        static_cast<uint8_t>(lhs) ^ static_cast<uint8_t>(rhs)
    );
    return lhs;
}

// Bitwise or assignment operator
constexpr Castle& operator|=(Castle& lhs, Castle rhs) noexcept {
    lhs = static_cast<Castle>(
        static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs)
    );
    return lhs;
}

// Bitwise left shift assignment operator
constexpr Castle& operator<<=(Castle& lhs, uint8_t shift) noexcept {
    lhs = static_cast<Castle>(
        static_cast<uint8_t>(lhs) << shift
    );
    return lhs;
}

// Bitwise right shift assignment operator
constexpr Castle& operator>>=(Castle& lhs, uint8_t shift) noexcept {
    lhs = static_cast<Castle>(
        static_cast<uint8_t>(lhs) >> shift
    );
    return lhs;
}

// Bitwise not operator
constexpr Castle operator~(Castle value) noexcept {
    return static_cast<Castle>(
        ~static_cast<uint8_t>(value)
    );
}

// Equality operator
constexpr bool operator==(Castle lhs, Castle rhs) noexcept {
    return static_cast<uint8_t>(lhs) == static_cast<uint8_t>(rhs);
}

// Inequality operator
constexpr bool operator!=(Castle lhs, Castle rhs) noexcept {
    return static_cast<uint8_t>(lhs) != static_cast<uint8_t>(rhs);
}

inline Castle castling_rights(Square square) noexcept
{
    static constexpr std::array<Castle, SQUARE_NB> CASTLING_RIGHTS = []() consteval
    {
        std::array<Castle, SQUARE_NB> arr {};
        arr.fill(Castle(0xFF));

        for (Color color : {Color::Red, Color::Blue, Color::Yellow, Color::Green})
        {
            for (Side side : {Side::KingSide, Side::QueenSide})
            {
                auto king_source = castle_squares(color, side)[0];
                auto rook_source = castle_squares(color, side)[2];

                arr[static_cast<uint8_t>(king_source)] &= static_cast<Castle>(~static_cast<uint8_t>(Castle(color, side)));
                arr[static_cast<uint8_t>(rook_source)] &= static_cast<Castle>(~static_cast<uint8_t>(Castle(color, side)));
            }
        }

        return arr;
    }();

    return CASTLING_RIGHTS[static_cast<uint8_t>(square)];
}

} // namespace athena