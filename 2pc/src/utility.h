#ifndef UTILITY_H
#define UTILITY_H

#include <sstream>
#include "fen.h"
#include "position.h"

namespace Talia
{
    
std::string formatNumber(uint64_t num);

std::vector<std::string> splitString(const std::string& input);

void printPosition(const Position& pos);

} // namespace

#endif