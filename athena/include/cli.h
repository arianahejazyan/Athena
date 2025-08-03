#ifndef CLI_H
#define CLI_H

// #include <CLI/CLI.hpp>
#include <iostream>
#include <cstring>
// #include "chess.h"
// #include "position.h"

#include "agent.h"
#include "utility.h"

namespace athena
{

class CommandLine
{
    private:
    Position pos;
    Agent agent;
    Setup setup;

    //
    // String<32> tokens;

    String<1024> line;
    int argc = 0;
    char* argv[32] = {}; // update location

    void handleUCI();
    void handleIsReady();
    void handleSetOption();
    void handleUCINewGame();
    void handleSetup();
    void handlePosition();
    void handleFEN();
    void handleGo();
    void handleStop();
    void handleQuit();
    void handlePerft();
    void handlePrint();

    public:
    void launch();
    void execute(String<1024> line);
};


    // class Engine
    // {
    // private:
    //     Position pos;
    //     CLI::App app{"Athena Engine CLI"};

    //     // Configuration
    //     bool debug;

    //     // Position options
    //     std::string position_mode;

    //     // Perft options
    //     int perft_depth;
    //     bool perft_full;
    //     bool perft_split;
    //     bool perft_cumulative;

    //     // Print options
    //     bool print_config = false;
    //     bool print_fen = false;
    //     bool print_ascii_pieces = false;

    //     // UCI commands
    //     void handleUCI();
    //     void handleIsReady();
    //     void handleSetOption();
    //     void handleUCINewGame();
    //     void handlePosition();
    //     void handleGo();
    //     void handleStop();
    //     void handleQuit();

    //     // Other commands
    //     void handlePerft();
    //     void handlePrint();
    //     // void handleConfig();

    // public:
    //     Engine();

    //     void launch();
    //     void execute(int argc, const char *argv[]);
    // };

} // namespace athena

#endif /* CLI_H */