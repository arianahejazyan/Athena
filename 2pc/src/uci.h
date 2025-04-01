#ifndef UCI_H
#define UCI_H

#include <iostream>
#include <sstream>
#include "fen.h"
#include "position.h"

namespace Talia
{

void handleUCI();
void handleIsReady();
void handlePosition(Position& pos, const std::vector<std::string> args);

} // namespace

#endif