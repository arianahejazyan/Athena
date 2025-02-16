#ifndef UCI_HPP
#define UCI_HPP

#include <iostream>
#include <sstream>
#include "gamestate.hpp"

using namespace std;
namespace ELSA
{

void handleUCI()
{
    cout << "id name ELSA-2PC" << endl;
    cout << "id author Ariana Hejazyan" << endl;
    cout << "uciok" << endl;
}

void handleCommand(GameState& state, const string& command)
{
    stringstream ss(command);
    string token;
    ss >> token;

    try {

    if (command == "uci") handleUCI();

    else throw invalid_argument("[CLI]: unknown command");

    } catch (const exception& e) {
        cout << e.what() << endl;
    }
}

}; // namespace

#endif