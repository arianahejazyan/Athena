#include <iostream>
#include <memory>
#include "engine.h"
#include "chess/position.h"
#include "chess/castle.h"
#include "option/option_combo.h"

namespace athena::core {

Engine::Engine() {
    setSetup("modern");
    setPosition(chess::Position::startpos(pos_.setup()));
    options_["setup"] = std::make_unique<ComboOption>("setup", "modern",
        std::vector<std::string>{"modern", "classic"},
        [this](const Option& o) {
            setSetup(static_cast<const ComboOption&>(o).getValue());
        });
}

void Engine::setOption(const std::string& name, const std::string& value) {
    auto it = options_.find(name);
    if (it != options_.end())
        it->second->setValue(value);
    else
        std::cout << "info string unknown option: " << name << "\n";
}

void Engine::setSetup(const std::string& setup) {
    pos_.set_setup(setup == "modern" ? 
        chess::Castle::Setup::Modern : 
        chess::Castle::Setup::Classic);
}

void Engine::setPosition(const std::string& fen) {
    pos_.init(fen);
}

void Engine::applyMove(const std::string& move) {
    pos_.make_move(move);
}

} // namespace athena