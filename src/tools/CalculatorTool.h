#pragma once
#include "Tool.h"
#include <string>

/*
 CalculatorTool - enhanced version
 - 支持自动分词（无需空格）
 - 支持 + - * / ^、括号、函数（sin cos tan log ln sqrt abs）
 - 支持一元负号
 - 支持命令: help, quit
*/
class CalculatorTool : public Tool {
public:
    std::string name() const override { return "Calculator"; }
    std::string description() const override { return "Expression calculator (+ - * / ^, funcs: sin cos tan log ln sqrt abs)"; }
    void run() override;

private:
    double evalExpr(const std::string& expr);

    // helper: convert infix -> RPN (tokens), then eval RPN
};

