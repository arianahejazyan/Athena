#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include "gamestate.h"

using namespace std;

namespace ELSA
{

void initGameState(GameState& state, const string fen);
void printGameState(const GameState& state);

}; // namespace

#endif