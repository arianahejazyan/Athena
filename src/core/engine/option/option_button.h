#pragma once

#include "option.h"

namespace athena::core {

class ButtonOption : public Option {
public:
    ButtonOption(std::string name, Callback cb) {
        this->name     = name;
        this->callback = cb;
    }

    std::string getType() const override { return "button"; }

    std::string serialize() const override {
        return "option name " + name + " type button";
    }

    void setValue(const std::string&) override {
        callback(*this);
    }
};

} // namespace athena