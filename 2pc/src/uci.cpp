#include <iostream>
#include <sstream>
#include "uci.h"
#include "utility.h"

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

    if (command == "uci")
        handleUCI();

    else if (command == "display" || command == "d")
        printGameState(state);
}

}; // namespace