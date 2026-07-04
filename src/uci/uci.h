#pragma once

#include <functional>
#include <iostream>
#include <unordered_map>
#include "engine.h"

namespace athena::uci {

class UCIEngine {
public:
    UCIEngine();
    void run(int argc, char** argv);

private:
    void register_commands();

    // UCI Commands
    void uci(std::istream& is);
    void isready(std::istream& is);
    void setoption(std::istream& is);
    void ucinewgame(std::istream& is);
    void position(std::istream& is);
    void go(std::istream& is);
    void stop(std::istream& is);
    void quit(std::istream& is);

    // Debug Commands
    void perft(std::istream& is);
    void print(std::istream& is);

    // Helper Functinos
    Search::Limits parse_limits(std::istream&);

    Engine engine_;
    std::unordered_map<std::string, std::function<void(std::istream&)>> commands_;
};

} // namespace athena