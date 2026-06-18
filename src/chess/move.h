#pragma once

#include "square.h"
#include "color.h"
#include "piece.h"
#include "castle.h"

namespace athena::chess {

class Move {
public: 
    enum class Policy : uint8_t {
        Normal = 0,
        Stride = 1,
        Evolve = 2,
        Enpass = 3,
        Castle = 4,
    };

    enum class Flag : uint8_t {
        Noisy = 0,
        Quiet = 1,
        Legal = 2,
    };

    constexpr Move() noexcept = default;
    constexpr Move(
        Square::ID   source, // 8 bits
        Square::ID   target, // 8 bits
        Piece::ID    evolve, // 3 bits
        Color::ID    enpass, // 3 bits
        Castle::Side castle, // 2 bits
        Move::Policy policy  // 3 bits
            ) noexcept : move_(
            (static_cast<uint32_t>(source) << 0 ) |
            (static_cast<uint32_t>(target) << 24) |
            (static_cast<uint32_t>(evolve) << 8 ) |
            (static_cast<uint32_t>(enpass) << 11) |
            (static_cast<uint32_t>(castle) << 14) |
            (static_cast<uint32_t>(policy) << 16) ) {}
 
    auto source() const noexcept { return static_cast<Square::ID  >((move_ & 255)        ); }
    auto target() const noexcept { return static_cast<Square::ID  >((move_ >> 24)        ); }
    auto evolve() const noexcept { return static_cast<Piece::ID   >((move_ >> 8 ) & 0b111); }
    auto enpass() const noexcept { return static_cast<Color::ID   >((move_ >> 11) & 0b111); }
    auto castle() const noexcept { return static_cast<Castle::Side>((move_ >> 14) & 0b001); }
    auto policy() const noexcept { return static_cast<Move::Policy>((move_ >> 16) & 0b111); } 
    
    std::string uci(bool board16x16 = false) const noexcept {
        std::string str = "";
        str += Square(source()).uci(board16x16);
        str += Square(target()).uci(board16x16);
        str += evolve() == Piece::ID::Empty ? "" : std::to_string(Piece(evolve()).uci());
        return str;
    }

private:
    uint32_t move_;
};

} // namespace athena