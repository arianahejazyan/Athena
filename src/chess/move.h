#pragma once

#include "square.h"
#include "color.h"
#include "piece.h"
#include "castle.h"

namespace athena
{

// Forward declaration
class Position;

// WARNING: Evolve can simultaneously be Enpass too
enum class MoveType : uint8_t
{
    Crawl  = 0,
    Slide  = 1,
    Push   = 2,
    Take   = 3,
    Stride = 4,
    Evolve = 5,
    Enpass = 6,
    Castle = 7,
};

enum class MoveFlag : uint8_t
{
    Noisy = 0,
    Quiet = 1,
};

class Move
{
    public:

    Move(Square source, Square target, MoveFlag flag, MoveType type) noexcept
        : encoded_(static_cast<uint8_t>(source) | (static_cast<uint8_t>(target) << 24) | (static_cast<uint32_t>(type) << 8)) {}

    Move(Square source, Square target, Side side) noexcept
        : encoded_(static_cast<uint8_t>(source) | (static_cast<uint8_t>(target) << 24) | (static_cast<uint32_t>(MoveType::Castle) << 8) | (static_cast<uint8_t>(side) << 18)) {}

    Move(Square source, Square target, Piece piece) noexcept
        : encoded_(static_cast<uint8_t>(source) | (static_cast<uint8_t>(target) << 24) | (static_cast<uint32_t>(MoveType::Evolve) << 8) | (static_cast<uint8_t>(piece) << 12)) {}

    Move(Square source, Square target, Color enpass) noexcept
        : encoded_(static_cast<uint8_t>(source) | (static_cast<uint8_t>(target) << 24) | (static_cast<uint32_t>(MoveType::Enpass) << 8) | (static_cast<uint8_t>(enpass) << 15)) {}

    Move(Move move, bool pseudo) noexcept
        : encoded_(move.encoded_ | (pseudo << 20)) {}

    Move() noexcept = default;

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

    bool pseudo() const noexcept {
        return encoded_ & (1 << 20);
    }

    void setpseudo(Square sq) noexcept {
        encoded_ |= ((source() == sq) << 20);
    }

    // UCI Conversion
    std::string uci(bool board16x16 = false) const noexcept;

    // Conversion operator
    explicit constexpr operator uint32_t() const noexcept { return encoded_; }

    private:
    uint32_t encoded_;
};

} // namespace athena