#pragma once

#include <cstdint>

namespace athena::chess {

class Color {
public: 
    enum class ID : uint8_t {
        Red    = 0,
        Blue   = 1,
        Yellow = 2,
        Green  = 3,
        None   = 4,
    };

    constexpr Color() noexcept = default;
    constexpr Color(ID id) noexcept : id_(id) {}

    Color(char c) noexcept {
        switch (c) {   
            case 'r': id_ = ID::Red   ; break;
            case 'b': id_ = ID::Blue  ; break;
            case 'y': id_ = ID::Yellow; break;
            case 'g': id_ = ID::Green ; break;
            default : id_ = ID::None  ; break;
        }
    }

    char uci() const noexcept {
        switch (id_) {   
            case ID::Red   : return 'r';
            case ID::Blue  : return 'b';
            case ID::Yellow: return 'y';
            case ID::Green : return 'g';
            default        : return '?';
        }
    }

    constexpr Color next() const noexcept { return Color(static_cast<ID>((static_cast<uint8_t>(id_) + 1) & 3)); }
    constexpr Color ally() const noexcept { return Color(static_cast<ID>((static_cast<uint8_t>(id_) + 2) & 3)); }
    constexpr Color prev() const noexcept { return Color(static_cast<ID>((static_cast<uint8_t>(id_) + 3) & 3)); }

    constexpr bool isSameSide(Color color) const noexcept { return (static_cast<uint8_t>(id_) & 1) == (static_cast<uint8_t>(color.id_) & 1); }
    constexpr bool isDiffSide(Color color) const noexcept { return (static_cast<uint8_t>(id_) & 1) != (static_cast<uint8_t>(color.id_) & 1); }
   
    constexpr ID id() const noexcept { return id_; }
    
private:
    ID id_;
};

inline constexpr bool operator==(Color lhs, Color rhs) noexcept { return lhs.id() == rhs.id(); }
inline constexpr bool operator!=(Color lhs, Color rhs) noexcept { return lhs.id() != rhs.id(); }

} // namespace athena