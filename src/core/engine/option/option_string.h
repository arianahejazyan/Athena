#pragma once

#include "option.h"

namespace athena::core {

class StringOption : public Option {
public:
    StringOption(std::string name, std::string def, Callback cb) {
        this->name     = std::move(name);
        this->callback = cb;
        defaultValue   = def;
        currentValue   = def;
    }
    
    auto getValue() const { return currentValue; }

    std::string getType() const override { return "string"; }

    std::string serialize() const override {
        return "option name " + name
            + " type string"
            + " default " + defaultValue;
    }

    void setValue(const std::string& val) override {
        currentValue = val;
        callback(*this);
    }

    operator std::string() const { return currentValue; }

private:
    std::string currentValue, defaultValue;
};

} // namespace athena