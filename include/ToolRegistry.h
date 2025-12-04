#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include "Tool.h"

class ToolRegistry {
public:
    static ToolRegistry& instance();

    bool registerTool(std::unique_ptr<Tool> tool);

    Tool* get(const std::string& name);
    std::vector<std::string> listTools() const;

private:
    std::unordered_map<std::string, std::unique_ptr<Tool>> tools;
};

