#pragma once

#include "position.h"
#include "thread.h"
namespace athena
{

    int negamax(Position &pos, Thread &thread, int alpha, int beta, int depth, int play = 0);

} // namespace athena