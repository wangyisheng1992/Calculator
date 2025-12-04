#include "TextEncryptTool.h"
#include "ConfigManager.h"
#include <iostream>

static std::string xor_crypt(const std::string& s, const std::string& key) {
    std::string out = s;
    for (size_t i = 0; i < s.size(); i++)
        out[i] = s[i] ^ key[i % key.size()];
    return out;
}

void TextEncryptTool::run() {
    auto& cfg = ConfigManager::instance().config();
    std::string method = cfg["encrypt"]["method"];
    int shift = cfg["encrypt"]["shift"];
    std::string key = cfg["encrypt"]["key"];

    while (true) {
        std::cout << "[EncryptTool] command (encrypt/decrypt/set/quit): ";
        std::string cmd;
        std::getline(std::cin, cmd);

        if (cmd == "quit") break;

        if (cmd.rfind("set key ", 0) == 0) {
            key = cmd.substr(8);
            continue;
        }

        if (cmd == "set method xor") method = "xor";
        if (cmd == "set method caesar") method = "caesar";

        if (cmd == "encrypt") {
            std::cout << "Enter text: ";
            std::string text;
            std::getline(std::cin, text);

            if (method == "xor")
                std::cout << xor_crypt(text, key) << "\n";
            else if (method == "caesar") {
                for (char& c : text) c += shift;
                std::cout << text << "\n";
            }
        }

        if (cmd == "decrypt") {
            std::cout << "Enter text: ";
            std::string text;
            std::getline(std::cin, text);

            if (method == "xor")
                std::cout << xor_crypt(text, key) << "\n";
            else if (method == "caesar") {
                for (char& c : text) c -= shift;
                std::cout << text << "\n";
            }
        }

        cfg["encrypt"]["method"] = method;
        cfg["encrypt"]["shift"] = shift;
        cfg["encrypt"]["key"] = key;
        ConfigManager::instance().save();
    }
}

