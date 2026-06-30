#pragma once

#include <iostream>
#include "chess/position.h"
#include "core/engine/engine.h"

namespace athena::cli {

class CLI {
public:
    CLI();
    int run(int argc, char** argv);

private:
    void registerCommands();

    // UCI Commands
    void uci(std::istream& is);
    void isready(std::istream& is);
    void setoption(std::istream& is);
    void ucinewgame(std::istream& is);
    void pos(std::istream& is);
    void go(std::istream& is);
    void stop(std::istream& is);
    void quit(std::istream& is);

    // Debug Commands
    void perft(std::istream& is);
    void print(std::istream& is);

    chess::Position pos_;
    core::Engine engine_;
    std::unordered_map<std::string, std::function<void(std::istream&)>> commands_;
};

} // namespace athena