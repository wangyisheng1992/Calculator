#include "UnitConverterTool.h"
#include <iostream>

void UnitConverterTool::run() {
    double v;
    std::cout << "Enter meters: ";
    std::cin >> v;
    std::cin.ignore();

    std::cout << v << " m = " << v * 100 << " cm\n";
    std::cout << v << " m = " << v * 1000 << " mm\n";
}

