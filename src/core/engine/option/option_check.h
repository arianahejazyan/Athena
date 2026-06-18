#pragma once

#include "option.h"

namespace athena::core {

class CheckOption : public Option {
public:
    CheckOption(std::string name, bool def, Callback cb) {
        this->name     = name;
        this->callback = cb;
        defaultValue   = def;
        currentValue   = def;
    }

    auto getValue() const { return currentValue; }
    
    std::string getType() const override { return "check"; }

    std::string serialize() const override {
        return "option name " + name
             + " type check"
             + " default " + (defaultValue ? "true" : "false");
    }

    void setValue(const std::string& val) override {
        currentValue = (val == "true");
        callback(*this);
    }

    operator bool() const { return currentValue; }

private:
    bool currentValue, defaultValue;
};

} // namespace athena