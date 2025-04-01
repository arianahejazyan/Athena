#include <CLI/CLI.hpp>
#include "commandline.h"
#include "movegen.h"
#include "fen.h"
#include "perft.h"
#include "uci.h"
#include "utility.h"

namespace Talia
{

CommandLine::CommandLine(): pos(fenToPosition(startpos)) {}

CommandLine::~CommandLine() = default;

void CommandLine::run()
{
    std::string command;
    while (std::getline(cin, command) && command != "quit") handleCommand(command);
}

void CommandLine::handleMakemove(std::vector<std::string>& args)
{
    std::string moveUci;

    CLI::App makemove{"Make a move"};
    makemove.add_option("move", moveUci, "Move in UCI format (e.g., e2e4)")->required();

    try
    {
        makemove.parse(args);

        StaticArray<Move, MAX_MOVES> moves;
        const auto size = genAllLegalMoves(pos, moves.begin());

        int idx = -1;
        for (int i = 0; i < size; ++i)
            if (moves[i].toUCI() == moveUci) idx = i;

        if (idx == -1)
            throw std::invalid_argument("[MAKEMOVE]: illegal move '" + moveUci + "'");
        else pos.makemove(moves[idx]), pos.moveHistory.push_back(moves[idx]);
    } 

    catch (const CLI::ParseError& e) { cout << makemove.help() << endl; }
}

void CommandLine::handleUndomove()
{
    if (pos.moveHistory.size() == 0)
        throw std::out_of_range("[UNDOMOVE]: no moves to undo, position is in the initial state");
    else 
        pos.undomove(pos.moveHistory.back()), pos.moveHistory.pop_back();
}

void CommandLine::handlePerft(std::vector<std::string>& args)
{
    int depth = 0;
    bool split = false;

    CLI::App perft{"Performance test at specified depth"};
    perft.add_option("depth", depth, "Search depth (1-9)")->required()->check(CLI::Range(1, 9));
    perft.add_flag("-s,--split", split, "Show node count for each move");

    try 
    {    
        perft.parse(args);
        printPerftResult(pos, depth, split);
    } 

    catch (const CLI::ParseError& e) { std::cout << perft.help() << std::endl; }
}

void CommandLine::handleCommand(const std::string& command)
{
    try
    {
        // Skip empty commands
        if (command.empty()) return;
        
        // Split the command line into tokens
        std::vector<std::string> tokens = splitString(command);

        // Extract the command name
        std::string name = tokens[0];

        // Extract the command args 
        std::vector<std::string> args(tokens.begin() + 1, tokens.end());

        if (command == "uci") 
            handleUCI();
        else if (command == "isready")
            handleIsReady();
        else if (name == "pos")
            handlePosition(pos, args);
        else if (name == "perft")
            handlePerft(args);
        else if (name == "makemove")
            handleMakemove(args);
        else if (name == "undomove")
            handleUndomove();
        else if (command == "print")
            printPosition(pos);

        else throw std::invalid_argument("[CLI]: unknown command name '" + name + "'");
        
    } catch (const std::exception& e) { std::cout << e.what() << std::endl; }
}

} // namespace