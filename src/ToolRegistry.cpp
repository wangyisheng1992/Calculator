#include "ToolRegistry.h"

ToolRegistry& ToolRegistry::instance() {
    static ToolRegistry inst;
    return inst;
}

bool ToolRegistry::registerTool(std::unique_ptr<Tool> tool) {
    std::string n = tool->name();
    tools[n] = std::move(tool);
    return true;
}

Tool* ToolRegistry::get(const std::string& name) {
    if (tools.count(name)) return tools[name].get();
    return nullptr;
}

std::vector<std::string> ToolRegistry::listTools() const {
    std::vector<std::string> names;
    for (auto& p : tools) names.push_back(p.first);
    return names;
}

