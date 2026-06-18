#pragma once

#include <iostream>
#include "core/engine/engine.h"

namespace athena::cli {

class CLI {
public:
    CLI() = default;
    void run(int argc, char *argv[]);

private:
    void uci();
    void isready();
    void setoption(std::istream& args);
    void ucinewGame();
    void pos(std::istream& args);
    void go(std::istream& args);
    void stop();
    void quit();

    core::Engine engine_;
};

} // namespace athena