#pragma once

#include <memory>
#include <string>
#include <map>
#include "option/option.h"
#include "chess/position.h"

namespace athena::core {

class Engine {
public:
    Engine();
    void setOption(const std::string& name, const std::string& value);
    void setPosition(const std::string& fen);
    void setSetup(const std::string& setup);
    void applyMove(const std::string& move);

    void print(bool board16x16 = false) { pos_.print(board16x16); }

private:
    chess::Position pos_;
    std::map<std::string, std::unique_ptr<Option>> options_;
};

} // namespace athena