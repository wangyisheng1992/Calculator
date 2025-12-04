#pragma once
#include <nlohmann/json.hpp>
#include <string>

class ConfigManager {
public:
    static ConfigManager& instance();

    nlohmann::json& config();
    void save();

private:
    ConfigManager();
    nlohmann::json cfg;
};

