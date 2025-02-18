#ifndef MAIN_CC
#define MAIN_CC

#include <iostream>
#include <sstream>
#include "gamestate.h"
#include "uci.h"
#include "utility.h"
#include "zobrist.h"

using namespace std;
using namespace ELSA;

int main(int argc, char* argv[])
{
    GameState state;

    initZobrist();

    initGameState(state, startpos);
    
    string command;
    while (getline(cin, command) && command != "quit") handleCommand(state, command);

    return 0;
}

#endif