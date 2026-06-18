#pragma once

#include <functional>
#include <string>

namespace athena::core {

class Option {
public:
    using Callback = std::function<void(const Option&)>;

    virtual std::string getType() const = 0;
    virtual std::string serialize() const = 0;
    virtual void        setValue(const std::string&) = 0;
    const std::string&  getName() const { return name; }
    virtual ~Option() = default;

protected:
    Callback callback;
    std::string name;
};

} // namespace athena