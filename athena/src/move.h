#pragma once

#include "square.h"
#include "color.h"
#include "piece.h"
#include "castle.h"

namespace athena
{

// Forward declaration
class Position;

enum class MoveType : uint8_t
{
    Simple = 0,
    Stride = 1,
    Enpass = 2,
    Castle = 3,
    Evolve = 4,
};

enum class MoveFlag : uint8_t
{
    Noisy = 0,
    Quiet = 1,
};

class Move
{
    public:

    Move(Square source, Square target, MoveType type)
        : encoded_(static_cast<uint8_t>(source) | (static_cast<uint8_t>(target) << 24) | (static_cast<uint32_t>(type) << 8)) {}

    consteval
    Move(Square source, Square target, Side side)
        : encoded_(static_cast<uint8_t>(source) | (static_cast<uint8_t>(target) << 24) | (static_cast<uint32_t>(MoveType::Castle) << 8) | (static_cast<uint8_t>(side) << 18)) {}

    Move(Square source, Square target, Piece piece)
        : encoded_(static_cast<uint8_t>(source) | (static_cast<uint8_t>(target) << 24) | (static_cast<uint32_t>(MoveType::Evolve) << 8) | (static_cast<uint8_t>(piece) << 12)) {}

    Move(Square source, Square target, Color enpass)
        : encoded_(static_cast<uint8_t>(source) | (static_cast<uint8_t>(target) << 24) | (static_cast<uint32_t>(MoveType::Enpass) << 8) | (static_cast<uint8_t>(enpass) << 15)) {}

    Move() = default;

    Square source() const noexcept { 
        return Square(encoded_ & 0xFF); 
    }

    Square target() const noexcept { 
        return Square(encoded_ >> 24); 
    }

    Side castle() const noexcept { 
        return Side((encoded_ >> 18) & 0x1);
    }

    Color enpass() const noexcept { 
        return Color((encoded_ >> 15) & 0x7); 
    }

    Piece evolve() const noexcept { 
        return Piece((encoded_ >> 12) & 0x7);
    }

    MoveType type() const noexcept { 
        return MoveType((encoded_ >> 8) & 0x7); 
    }

    MoveFlag flag() const noexcept { 
        return MoveFlag((encoded_ >> 11) & 0x1); 
    }

    // UCI Conversion
    std::string uci() const noexcept;

    // Conversion operator
    explicit constexpr operator uint32_t() const noexcept { return encoded_; }

    private:
    uint32_t encoded_;
};

} // namespace athena