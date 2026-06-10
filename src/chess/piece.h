#pragma once

#include <cstdint>

namespace athena::chess {

class Piece {
public: 
    enum class ID : uint8_t {
        King   = 0,
        Knight = 1,
        Bishop = 2,
        Rook   = 3,
        Queen  = 4,
        Pawn   = 5,
        Empty  = 6,
        Stone  = 7,
    };

    constexpr Piece() noexcept = default;
    constexpr Piece(ID id) noexcept : id_(id) {}

    Piece(char c) noexcept {
        switch (c) {
            case 'P': id_ = ID::Pawn  ; break;
            case 'N': id_ = ID::Knight; break;
            case 'B': id_ = ID::Bishop; break;
            case 'R': id_ = ID::Rook  ; break;
            case 'Q': id_ = ID::Queen ; break;
            case 'K': id_ = ID::King  ; break;
            default : id_ = ID::Empty ; break;
        }    
    }

    char uci() const noexcept {
        switch (id_) {
            case ID::Pawn  : return 'P';
            case ID::Knight: return 'N';
            case ID::Bishop: return 'B';
            case ID::Rook  : return 'R';
            case ID::Queen : return 'Q';
            case ID::King  : return 'K';
            default        : return '?';
        }
    }

    constexpr ID id() const noexcept { return id_; }

private:
    ID id_;
};

inline constexpr bool operator==(Piece lhs, Piece rhs) noexcept { return lhs.id() == rhs.id(); }
inline constexpr bool operator!=(Piece lhs, Piece rhs) noexcept { return lhs.id() != rhs.id(); }

} // namespace athena