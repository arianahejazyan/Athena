#pragma once

#include <iostream>
#include "option.h"

namespace athena::core {

class SpinOption : public Option {
public:
    SpinOption(std::string name, int def, int min, int max, Callback cb) {
        this->name     = name;
        this->callback = cb;
        defaultValue   = def;
        currentValue   = def;
        this->min      = min;
        this->max      = max;
    }

    auto getValue() const { return currentValue; }
    
    std::string getType() const override { return "spin"; }

    std::string serialize() const override {
        return "option name " + name
                + " type spin"
                + " default " + std::to_string(defaultValue)
                + " min "     + std::to_string(min)
                + " max "     + std::to_string(max);
    }

    void setValue(const std::string& val) override {
        int v = std::stoi(val);
        if (v >= min && v <= max) {
            currentValue = v;
            callback(*this);
        } else {
            std::cout << "info string option " << name
                      << " value " << v
                      << " out of range [" << min << ", " << max << "]\n";
        }
    }

    operator int() const { return currentValue; }

private:
    int currentValue, defaultValue, min, max;
};

} // namespace athena