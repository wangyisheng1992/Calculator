#pragma once
#include <string>

class Tool {
public:
    virtual ~Tool() = default;
    virtual std::string name() const = 0;
    virtual std::string description() const { return ""; }
    virtual void run() = 0;
};

