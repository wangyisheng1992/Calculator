#include "CalculatorTool.h"
#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include <string>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <unordered_map>
#include <functional>
#include <limits>

/*
 Implementation details:
 - Tokenizer produces numbers, identifiers (functions), operators, parentheses.
 - Uses Shunting-Yard to produce RPN.
 - Evaluates RPN.
 - Throws runtime_error on malformed expressions.
*/

namespace {

enum TokenType { T_NUMBER, T_OP, T_LPAREN, T_RPAREN, T_IDENT };

struct Token {
    TokenType type;
    std::string text; // for number: textual form; for op/ident: symbol/name
    double value{};   // when type == T_NUMBER
};

std::vector<Token> tokenize(const std::string& s) {
    std::vector<Token> out;
    size_t i = 0;
    while (i < s.size()) {
        char c = s[i];
        if (std::isspace((unsigned char)c)) { ++i; continue; }

        if (std::isdigit((unsigned char)c) || c == '.') {
            size_t j = i;
            while (j < s.size() && (std::isdigit((unsigned char)s[j]) || s[j] == '.')) ++j;
            std::string num = s.substr(i, j - i);
            Token t; t.type = T_NUMBER; t.text = num;
            try {
                t.value = std::stod(num);
            } catch (...) { throw std::runtime_error("Invalid number: " + num); }
            out.push_back(std::move(t));
            i = j;
            continue;
        }

        if (std::isalpha((unsigned char)c)) {
            size_t j = i;
            while (j < s.size() && std::isalpha((unsigned char)s[j])) ++j;
            std::string id = s.substr(i, j - i);
            Token t; t.type = T_IDENT; t.text = id;
            out.push_back(std::move(t));
            i = j;
            continue;
        }

        // operators and parentheses
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            Token t; t.type = T_OP; t.text = std::string(1, c);
            out.push_back(std::move(t));
            ++i;
            continue;
        }

        if (c == '(') { out.push_back({T_LPAREN, "("}); ++i; continue; }
        if (c == ')') { out.push_back({T_RPAREN, ")"}); ++i; continue; }

        throw std::runtime_error(std::string("Unknown character: ") + c);
    }
    return out;
}

int precedence(const std::string& op) {
    if (op == "^") return 4;
    if (op == "*" || op == "/") return 3;
    if (op == "+" || op == "-") return 2;
    return 0;
}

// right associative?
bool is_right_associative(const std::string& op) {
    return op == "^";
}

double apply_binary(const std::string& op, double a, double b) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") {
        if (b == 0.0) throw std::runtime_error("Division by zero");
        return a / b;
    }
    if (op == "^") return std::pow(a, b);
    throw std::runtime_error("Unknown binary op: " + op);
}

double apply_function(const std::string& name, double v) {
    static const std::unordered_map<std::string, std::function<double(double)>> funcs = {
        {"sin", [](double x){ return std::sin(x); }},
        {"cos", [](double x){ return std::cos(x); }},
        {"tan", [](double x){ return std::tan(x); }},
        {"sqrt", [](double x){ if (x < 0) throw std::runtime_error("sqrt of negative"); return std::sqrt(x); }},
        {"abs",  [](double x){ return std::fabs(x); }},
        {"log",  [](double x){ if (x <= 0) throw std::runtime_error("log of non-positive"); return std::log10(x); }},
        {"ln",   [](double x){ if (x <= 0) throw std::runtime_error("ln of non-positive"); return std::log(x); }}
    };
    auto it = funcs.find(name);
    if (it == funcs.end()) throw std::runtime_error("Unknown function: " + name);
    return it->second(v);
}

} // namespace

double CalculatorTool::evalExpr(const std::string& expr) {
    auto tokens = tokenize(expr);
    // Shunting-yard -> output queue (as tokens)
    std::vector<Token> output;
    std::vector<Token> ops; // operator stack (store tokens for ops and funcs and paren)
    // We need to handle unary minus: when '-' appears at start or after '(' or another operator -> unary
    for (size_t i = 0; i < tokens.size(); ++i) {
        Token t = tokens[i];
        if (t.type == T_NUMBER) {
            output.push_back(t);
        } else if (t.type == T_IDENT) {
            // function identifier -> push to ops
            ops.push_back(t);
        } else if (t.type == T_OP) {
            std::string op = t.text;
            // handle unary minus: represent as "u-" and handle as function (unary negation)
            bool is_unary = false;
            if (op == "-") {
                if (i == 0) is_unary = true;
                else {
                    Token prev = tokens[i-1];
                    if (prev.type == T_OP || prev.type == T_LPAREN) is_unary = true;
                    if (prev.type == T_IDENT) {
                        // e.g., sin -2 (should be treated as binary? usually unary)
                    }
                }
            }
            if (is_unary) {
                // treat as function "neg"
                ops.push_back({T_IDENT, "neg"});
                continue;
            }

            while (!ops.empty()) {
                Token top = ops.back();
                if (top.type == T_IDENT) {
                    // functions have higher precedence; pop to output
                    output.push_back(top);
                    ops.pop_back();
                } else if (top.type == T_OP) {
                    std::string topop = top.text;
                    if ((is_right_associative(op) && precedence(op) < precedence(topop)) ||
                        (!is_right_associative(op) && precedence(op) <= precedence(topop))) {
                        output.push_back(top);
                        ops.pop_back();
                    } else break;
                } else break;
            }
            ops.push_back(t);
        } else if (t.type == T_LPAREN) {
            ops.push_back(t);
        } else if (t.type == T_RPAREN) {
            bool foundLeft = false;
            while (!ops.empty()) {
                Token top = ops.back();
                ops.pop_back();
                if (top.type == T_LPAREN) { foundLeft = true; break; }
                output.push_back(top);
            }
            if (!foundLeft) throw std::runtime_error("Mismatched parentheses");
            // if function on top of ops, pop it to output
            if (!ops.empty() && ops.back().type == T_IDENT) {
                output.push_back(ops.back());
                ops.pop_back();
            }
        }
    }

    while (!ops.empty()) {
        Token top = ops.back();
        ops.pop_back();
        if (top.type == T_LPAREN || top.type == T_RPAREN) throw std::runtime_error("Mismatched parentheses");
        output.push_back(top);
    }

    // Evaluate RPN
    std::stack<double> st;
    for (auto &tk : output) {
        if (tk.type == T_NUMBER) st.push(tk.value);
        else if (tk.type == T_OP) {
            if (st.size() < 2) throw std::runtime_error("Invalid expression (binary op)");
            double b = st.top(); st.pop();
            double a = st.top(); st.pop();
            st.push(apply_binary(tk.text, a, b));
        } else if (tk.type == T_IDENT) {
            // either function like sin, or unary neg "neg"
            if (tk.text == "neg") {
                if (st.empty()) throw std::runtime_error("Invalid expression (neg)");
                double v = st.top(); st.pop();
                st.push(-v);
            } else {
                if (st.empty()) throw std::runtime_error("Invalid expression (func)");
                double v = st.top(); st.pop();
                st.push(apply_function(tk.text, v));
            }
        } else {
            throw std::runtime_error("Unexpected token in RPN eval");
        }
    }

    if (st.empty()) throw std::runtime_error("Empty expression");
    double result = st.top(); st.pop();
    if (!st.empty()) throw std::runtime_error("Invalid expression (extra values)");
    return result;
}

void CalculatorTool::run() {
    std::cout << "\n=== Calculator ===\n";
    std::cout << "支持运算：+ - * / ^ 以及函数 sin cos tan sqrt log ln abs\n";
    std::cout << "示例: 3+4*2/(1-5)^2  或 sin(3.14/2)  或 -2^2\n";
    std::cout << "命令: help, quit\n";

    while (true) {
        std::cout << "> ";
        std::string line;
        if (!std::getline(std::cin, line)) break;
        if (line.empty()) continue;
        if (line == "quit") break;
        if (line == "help") {
            std::cout << "输入表达式后回车计算。支持: + - * / ^, 函数: sin cos tan sqrt log ln abs\n";
            continue;
        }
        try {
            double res = evalExpr(line);
            // print with good precision; if integer-like show without decimal?
            if (std::fabs(res - std::round(res)) < 1e-12) std::cout << (long long)std::llround(res) << "\n";
            else std::cout << res << "\n";
        } catch (const std::exception& e) {
            std::cout << "错误: " << e.what() << "\n";
        }
    }
}

