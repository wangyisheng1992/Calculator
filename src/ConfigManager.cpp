#include "ConfigManager.h"
#include <fstream>

ConfigManager::ConfigManager() {
    std::ifstream f("config.json");
    if (f.is_open()) {
        f >> cfg;
    } else {
        cfg = nlohmann::json::object();
    }
}

ConfigManager& ConfigManager::instance() {
    static ConfigManager inst;
    return inst;
}

nlohmann::json& ConfigManager::config() {
    return cfg;
}

void ConfigManager::save() {
    std::ofstream f("config.json");
    f << cfg.dump(4);
}

