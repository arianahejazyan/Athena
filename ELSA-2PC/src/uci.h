#ifndef UCI_H
#define UCI_H

#include <string>
#include "gamestate.h"

using namespace std;

namespace ELSA
{

const string startpos("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

void handleUCI();
void handleCommand(GameState& state, const string& command);

}; // namespace

#endif