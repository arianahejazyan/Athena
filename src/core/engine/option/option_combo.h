#pragma once

#include <iostream>
#include "option.h"

namespace athena::core {

class ComboOption : public Option {
    public:
    ComboOption(std::string name, std::string def, std::vector<std::string> choices, Callback cb) {
        this->name     = std::move(name);
        this->callback = cb;
        defaultValue   = def;
        currentValue   = def;
        validChoices   = std::move(choices);
    }

    auto getValue() const { return currentValue; }

    std::string getType() const override { return "combo"; }

    std::string serialize() const override {
        std::string result = "option name " + name
                            + " type combo"
                            + " default " + defaultValue;
        for (const auto& choice : validChoices)
            result += " var " + choice;
        return result;
    }

    void setValue(const std::string& val) override {
        if (!isValid(val)) {
            std::cout << "info string invalid value '" << val
                        << "' for option " << name << ". Valid: ";
            for (const auto& c : validChoices)
                std::cout << c << " ";
            std::cout << "\n";
            return;
        }
        currentValue = val;
        callback(*this);
    }

    operator std::string() const { return currentValue; }

    const std::vector<std::string>& getChoices() const { return validChoices; }

private:
    bool isValid(const std::string& val) const {
        return std::find(validChoices.begin(), validChoices.end(), val) != validChoices.end();
    }

    std::string              currentValue, defaultValue;
    std::vector<std::string> validChoices;
};
  
} // namespace athena