#pragma once

// #include <CLI/CLI.hpp>
#include <vector>
#include <string>
#include "chess.h"
#include "position.h"

// #include "perft.h"

namespace athena
{

class CLI
{
    public:
    CLI();

    void launch();
    void execute();

    private:
    std::string line_;
    std::vector<std::string_view> tokens_;

    // UCI commands
    void handleUCI();
    void handleIsReady();
    void handleSetOption();
    void handleUCINewGame();
    void handlePos();
    void handleGo();
    void handleStop();
    void handleQuit();

    // Other commands
    void handlePerft();
    void handlePrint();
    void handleFen();
    void handleMoveGen();

    private:
    Position pos_;

    /// @brief CLI11 application instance for command-line parsing
    // CLI::App app{"Athena Engine CLI"};

    // /// Configurations
    // Position pos;
    // Perft perft;
    // bool debug;
    // Setup setup;
    // std::string pos_mode;
    // std::vector<std::string> moves;
};

} // namespace athena