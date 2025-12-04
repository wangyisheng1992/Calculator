#include <iostream>
#include "ToolRegistry.h"

int main() {
    auto& reg = ToolRegistry::instance();

    while (true) {
        std::cout << "\n=== Personal Utility Suite ===\n";
        std::cout << "Available tools:\n";

        for (auto& name : reg.listTools())
            std::cout << " - " << name << "\n";

        std::cout << "\nEnter tool name (or quit): ";
        std::string name;
        std::getline(std::cin, name);

        if (name == "quit") break;

        Tool* t = reg.get(name);
        if (!t) {
            std::cout << "Tool not found!\n";
            continue;
        }

        t->run();
    }
    return 0;
}

