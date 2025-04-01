#ifndef PERTF_H
#define PERTF_H

#include <cstdint>
#include "position.h"

namespace Talia
{

[[nodiscard]] uint64_t perft(Position& pos, int depth);

[[nodiscard]] std::vector<std::pair<Move, uint64_t>> splitPerft(Position& pos, int depth);

void printPerftResult(const Position& pos, const int depth, const bool split);

} // namespace

#endif