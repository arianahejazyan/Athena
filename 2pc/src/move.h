#ifndef MOVE_H
#define MOVE_H

#include <string>
#include "chess.h"
#include "types.h"

namespace Talia
{

struct Move
{
    private:

        u16 encoded;

    public:

        Move() {}

        Move(Square from, Square to, u16 nature): encoded((from << 10) | (to << 4) | nature) {}

        ~Move() = default;

        [[nodiscard]] inline Square source() const noexcept
        {
            return static_cast<Square>((encoded >> 10) & 0x3fu);
        }

        [[nodiscard]] inline Square target() const noexcept
        {
            return static_cast<Square>((encoded >> 4) & 0x3fu);
        }

        [[nodiscard]] inline MoveFlag nature() const noexcept
        {
            return static_cast<MoveFlag>(encoded & 0xfu);
        }

        [[nodiscard]] std::string toUCI() const noexcept;
};

} // namespace

#endif