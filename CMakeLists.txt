cmake_minimum_required(VERSION 3.10)
project(PersonalUtilitySuite)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(PersonalUtilitySuite
    src/main.cpp
    src/ToolRegistry.cpp
    src/ConfigManager.cpp
    src/registerTools.cpp

    src/tools/CalculatorTool.cpp
    src/tools/ColorPickerTool.cpp
    src/tools/TextEncryptTool.cpp
    src/tools/TextStatsTool.cpp
    src/tools/UnitConverterTool.cpp
)

find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)

target_include_directories(PersonalUtilitySuite PRIVATE include)

target_link_libraries(PersonalUtilitySuite
    PRIVATE nlohmann_json::nlohmann_json
    PRIVATE spdlog::spdlog
)

