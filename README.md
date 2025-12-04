# Personal Utility Suite

A simple console-based personal utility suite in C++17.

## Features

- Modular tool system: easily add new tools
- Example tools:
  - Calculator: simple + - * / calculator
  - Text Stats: counts characters and words

## Project Structure

.
├── CMakeLists.txt
├── config.json
├── include
│   ├── ConfigManager.h
│   ├── Tool.h
│   └── ToolRegistry.h
├── README.md
└── src
    ├── ConfigManager.cpp
    ├── main.cpp
    ├── registerTools.cpp
    ├── ToolRegistry.cpp
    └── tools
        ├── CalculatorTool.cpp
        ├── CalculatorTool.h
        ├── ColorPickerTool.cpp
        ├── ColorPickerTool.h
        ├── TextEncryptTool.cpp
        ├── TextEncryptTool.h
        ├── TextStatsTool.cpp
        ├── TextStatsTool.h
        ├── UnitConverterTool.cpp
        └── UnitConverterTool.h


## Build

```bash
mkdir -p build && cd build
cmake ..
make -j$(nproc)
./PersonalUtilitySuite
```

## Usage

- Run the program and select a tool from the menu:
```vbnet
=== Personal Utility Suite ===
1. Calculator - Simple + - * / calculator
2. Text Stats - Count characters and words
0. Exit
Select option:
```

- 计算器示例：

  - 3+4*2/(1-5)^2

  - sin(3.1415926/2)

  - -2^2（注意：这里实现的优先级会把 -2^2 视为 -(2^2)，返回 -4）

  - help / quit

- 文本工具示例：

  - 直接粘入多行文本，按空行结束，程序会显示 summary 并提示继续或退出。

  - 输入 quit 可随时返回主菜单。
















