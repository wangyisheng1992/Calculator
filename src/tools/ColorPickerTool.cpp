#include "ColorPickerTool.h"
#include <iostream>

void ColorPickerTool::run() {
    int r, g, b;
    std::cout << "Enter RGB (0-255): ";
    std::cin >> r >> g >> b;
    std::cin.ignore();

    std::cout << "Color: RGB(" << r << "," << g << "," << b << ")\n";
}

