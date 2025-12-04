#pragma once
#include "Tool.h"
#include <iostream>

class ColorPickerTool : public Tool {
public:
    std::string name() const override { return "Color Picker"; }
    std::string description() const override { return "Pick RGB values"; }
    void run() override;
};

