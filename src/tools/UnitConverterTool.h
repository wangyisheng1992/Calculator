#pragma once
#include "Tool.h"
#include <iostream>

class UnitConverterTool : public Tool {
public:
    std::string name() const override { return "Unit Converter"; }
    std::string description() const override { return "Convert meters to cm/mm"; }
    void run() override;
};

