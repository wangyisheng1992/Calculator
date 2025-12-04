#pragma once
#include "Tool.h"
#include <string>

class TextEncryptTool : public Tool {
public:
    std::string name() const override { return "Text Encrypt"; }
    std::string description() const override { return "Base64 / Caesar / XOR encryption"; }
    void run() override;
};

