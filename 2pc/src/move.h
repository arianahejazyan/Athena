#ifndef MOVE_H
#define MOVE_H

#include "chess.h"

namespace ELSA
{

struct Move
{
    private:

        const u16 encoded;

    public:

        Move(Square from, Square to, u16 nature);

        Square source() const;
        Square target() const;
        Square nature() const;
};

}; // namespace

#endif