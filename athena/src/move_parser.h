#pragma once

#include <string>
#include <string_view>
#include "position.h"
#include "move.h"

namespace athena
{

bool parse_move_uci(const Position& pos, std::string_view uci, Move& out_move, std::string* error = nullptr);

} // namespace athena
