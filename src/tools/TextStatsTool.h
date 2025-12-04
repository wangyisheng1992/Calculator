#pragma once
#include "Tool.h"
#include <string>

/*
 TextStatsTool - enhanced
 - 支持多行输入（直到空行结束）
 - 命令:
     summary -> 输出统计信息
     quit    -> 返回主菜单
 - 输出: 字符总数、字符（不含空白）、单词数、句子数、平均单词长度、前 N 常见词
*/
class TextStatsTool : public Tool {
public:
    std::string name() const override { return "Text Stats"; }
    std::string description() const override { return "Count characters/words/sentences and top words"; }
    void run() override;
};

