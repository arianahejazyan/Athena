#pragma once

#include <array>
#include "color.h"
#include "constants.h"
#include "square.h"

namespace athena::chess {

class Castle {
public: 
    enum class Side : uint8_t 
    {
        KingSide  = 0,
        QueenSide = 1,
    };

    enum class Setup : uint8_t
    {
        Modern  = 0,
        Classic = 1,
    };

    using ID = uint8_t;

    constexpr Castle() noexcept = default;
    constexpr Castle(ID id) noexcept : id_(id) {}
    constexpr Castle(Color::ID color, Side side) noexcept 
        : id_(static_cast<uint8_t>(1 << (static_cast<int>(side) * COLOR_NB + static_cast<int>(color)))) {}

    Castle(std::string_view sv, Side side) noexcept {
        id_ = ID(0);
        if (sv[0] == '1') id_ |= Castle(Color::ID::Red   , side).id();
        if (sv[2] == '1') id_ |= Castle(Color::ID::Blue  , side).id();
        if (sv[4] == '1') id_ |= Castle(Color::ID::Yellow, side).id();
        if (sv[6] == '1') id_ |= Castle(Color::ID::Green , side).id();
    }

    std::string uci(Side side) const noexcept {
        std::string output = "";
        output += (id_ & Castle(Color::ID::Red   , side).id() ? '1' : '0'); output += ',';
        output += (id_ & Castle(Color::ID::Blue  , side).id() ? '1' : '0'); output += ',';
        output += (id_ & Castle(Color::ID::Yellow, side).id() ? '1' : '0'); output += ',';
        output += (id_ & Castle(Color::ID::Green , side).id() ? '1' : '0');
        return output; 
    }

    constexpr bool has(Castle other) const noexcept { return (id_ & other.id_) != 0; }
    constexpr void set(Castle other) noexcept { id_ |=  other.id_; }
    constexpr void pop(Castle other) noexcept { id_ &= ~other.id_; }

    constexpr ID id() const noexcept { return id_; }

private:
    ID id_;

    alignas(64)
    static constexpr 
    std::array<Square::ID, SETUP_NB * COLOR_NB * SIDE_NB * 8> table_ = 
    {
        Square::ID::I2 , Square::ID::K2 , Square::ID::L2 , Square::ID::J2 ,
        Square::ID::I2 , Square::ID::J2 , Square::ID::K2 , Square::ID::K2 ,
        Square::ID::I2 , Square::ID::G2 , Square::ID::E2 , Square::ID::H2 ,
        Square::ID::I2 , Square::ID::H2 , Square::ID::G2 , Square::ID::F2 ,
        Square::ID::B9 , Square::ID::B11, Square::ID::B12, Square::ID::B10,
        Square::ID::B9 , Square::ID::B10, Square::ID::B11, Square::ID::B11,
        Square::ID::B9 , Square::ID::B7 , Square::ID::B5 , Square::ID::B8 ,
        Square::ID::B9 , Square::ID::B8 , Square::ID::B7 , Square::ID::B6 ,
        Square::ID::H15, Square::ID::F15, Square::ID::E15, Square::ID::G15,
        Square::ID::H15, Square::ID::G15, Square::ID::F15, Square::ID::F15,
        Square::ID::H15, Square::ID::J15, Square::ID::L15, Square::ID::I15,
        Square::ID::H15, Square::ID::I15, Square::ID::J15, Square::ID::K15, 
        Square::ID::O8 , Square::ID::O6 , Square::ID::O5 , Square::ID::O7 ,
        Square::ID::O8 , Square::ID::O7 , Square::ID::O6 , Square::ID::O6 ,
        Square::ID::O8 , Square::ID::O10, Square::ID::O12, Square::ID::O9 ,
        Square::ID::O8 , Square::ID::O9 , Square::ID::O10, Square::ID::O11,
        Square::ID::I2 , Square::ID::K2 , Square::ID::L2 , Square::ID::J2 ,
        Square::ID::I2 , Square::ID::J2 , Square::ID::K2 , Square::ID::K2 ,
        Square::ID::I2 , Square::ID::G2 , Square::ID::E2 , Square::ID::H2 ,
        Square::ID::I2 , Square::ID::H2 , Square::ID::G2 , Square::ID::F2 ,
        Square::ID::B8 , Square::ID::B6 , Square::ID::B5 , Square::ID::B7 ,
        Square::ID::B8 , Square::ID::B7 , Square::ID::B6 , Square::ID::B6 ,
        Square::ID::B8 , Square::ID::B10, Square::ID::B12, Square::ID::B9 ,
        Square::ID::B8 , Square::ID::B9 , Square::ID::B10, Square::ID::B11,
        Square::ID::H15, Square::ID::F15, Square::ID::E15, Square::ID::G15,
        Square::ID::H15, Square::ID::G15, Square::ID::F15, Square::ID::F15,
        Square::ID::H15, Square::ID::J15, Square::ID::L15, Square::ID::I15,
        Square::ID::H15, Square::ID::I15, Square::ID::J15, Square::ID::K15, 
        Square::ID::O9 , Square::ID::O11, Square::ID::O12, Square::ID::O10,
        Square::ID::O9 , Square::ID::O10, Square::ID::O11, Square::ID::O11,
        Square::ID::O9 , Square::ID::O7 , Square::ID::O5 , Square::ID::O8 ,
        Square::ID::O9 , Square::ID::O8 , Square::ID::O7 , Square::ID::O6 , 
    };

private:
    static Square::ID square(Setup setup, Color::ID color, Side side,  int idx = 0) noexcept {
        return table_[
            64 * static_cast<int>(setup) +
            16 * static_cast<int>(color) + 
            8  * static_cast<int>(side ) + idx];
    }

public:
    static auto king_source(Setup setup, Color::ID color, Side side, int idx = 0) noexcept { return square(setup, color, side, idx + 0); }
    static auto king_target(Setup setup, Color::ID color, Side side, int idx = 0) noexcept { return square(setup, color, side, idx + 1); }
    static auto rook_source(Setup setup, Color::ID color, Side side, int idx = 0) noexcept { return square(setup, color, side, idx + 2); }
    static auto rook_target(Setup setup, Color::ID color, Side side, int idx = 0) noexcept { return square(setup, color, side, idx + 3); }
    static auto safe_square(Setup setup, Color::ID color, Side side, int idx = 0) noexcept { return square(setup, color, side, idx + 4); }
    static auto path_square(Setup setup, Color::ID color, Side side, int idx = 0) noexcept { return square(setup, color, side, idx + 5); }
};

inline constexpr bool operator==(Castle lhs, Castle rhs) noexcept { return lhs.id() == rhs.id(); }
inline constexpr bool operator!=(Castle lhs, Castle rhs) noexcept { return lhs.id() != rhs.id(); }

} // namespace athena