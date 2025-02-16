#ifndef MAIN_HPP
#define MAIN_HPP

#include "gamestate.hpp"
#include "uci.hpp"
#include "utility.hpp"

using namespace std;
using namespace ELSA;

int main(int argc, char* argv[])
{
    GameState state;

    initGameState(state);
    
    string command;
    while (getline(cin, command) && command != "quit") handleCommand(state, command);

    return 0;
}

#endif