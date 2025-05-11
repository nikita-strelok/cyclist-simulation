#pragma once
#include <functional>
#include <string>
#include <vector>
#include <stdexcept>
#include <cmath>

enum class OpType
{
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    SQRT,
    SIN,
    COS,
    VALUE,
    VAR,
    PI
};

struct Operation
{
    OpType type;
    double value;
};

class ExpressionParser
{
    std::string expr;
    size_t pos = 0;
    std::vector<Operation> operations;

    char peek()
    {
        while (pos < expr.size() && isspace(expr[pos]))
            ++pos;
        return pos < expr.size() ? expr[pos] : '\0';
    }

    char get()
    {
        char c = peek();
        if (pos < expr.size())
            ++pos;
        return c;
    }

    double parseNumber()
    {
        size_t start = pos;
        while (isdigit(peek()) || peek() == '.')
            get();
        return std::stod(expr.substr(start, pos - start));
    }

    void parsePrimary()
    {
        char c = get();
        if (c == '(')
        {
            parseExpression();
            if (get() != ')')
                throw std::runtime_error("Mismatched parentheses");
        }
        else if (c == 'x')
        {
            operations.push_back({OpType::VAR, 0});
        }
        else if (isdigit(c) || c == '.')
        {
            --pos;
            operations.push_back({OpType::VALUE, parseNumber()});
        }
        else if (c == 'p' && expr.substr(pos, 1) == "i")
        {
            ++pos;
            operations.push_back({OpType::PI, M_PI});
        }
        else if (expr.substr(pos - 1, 3) == "sin")
        {
            pos += 2;
            parseFactor();
            operations.push_back({OpType::SIN, 0});
        }
        else if (expr.substr(pos - 1, 3) == "cos")
        {
            pos += 2;
            parseFactor();
            operations.push_back({OpType::COS, 0});
        }
        else if (expr.substr(pos - 1, 4) == "sqrt")
        {
            pos += 3;
            parseFactor();
            operations.push_back({OpType::SQRT, 0});
        }
        else
        {
            throw std::runtime_error("Unexpected character");
        }
    }

    void parseFactor()
    {
        parsePrimary();
        while (peek() == '^')
        {
            get();
            parseFactor();
            operations.push_back({OpType::POW, 0});
        }
    }

    void parseTerm()
    {
        parseFactor();
        while (true)
        {
            char op = peek();
            if (op == '*' || op == '/')
            {
                get();
                parseFactor();
                operations.push_back({op == '*' ? OpType::MUL : OpType::DIV, 0});
            }
            else
            {
                break;
            }
        }
    }

    void parseExpression()
    {
        parseTerm();
        while (true)
        {
            char op = peek();
            if (op == '+' || op == '-')
            {
                get();
                parseTerm();
                operations.push_back({op == '+' ? OpType::ADD : OpType::SUB, 0});
            }
            else
            {
                break;
            }
        }
    }

public:
    ExpressionParser(const std::string &expression) : expr(expression) {}

    std::function<double(double)> compile()
    {
        pos = 0;
        operations.clear();
        parseExpression();
        return [this](double x)
        {
            std::vector<double> stack;
            for (const auto &op : operations)
            {
                switch (op.type)
                {
                case OpType::VALUE:
                    stack.push_back(op.value);
                    break;
                case OpType::VAR:
                    stack.push_back(x);
                    break;
                case OpType::PI:
                    stack.push_back(M_PI);
                    break;
                case OpType::ADD:
                {
                    double b = stack.back();
                    stack.pop_back();
                    double a = stack.back();
                    stack.pop_back();
                    stack.push_back(a + b);
                    break;
                }
                case OpType::SUB:
                {
                    double b = stack.back();
                    stack.pop_back();
                    double a = stack.back();
                    stack.pop_back();
                    stack.push_back(a - b);
                    break;
                }
                case OpType::MUL:
                {
                    double b = stack.back();
                    stack.pop_back();
                    double a = stack.back();
                    stack.pop_back();
                    stack.push_back(a * b);
                    break;
                }
                case OpType::DIV:
                {
                    double b = stack.back();
                    stack.pop_back();
                    double a = stack.back();
                    stack.pop_back();
                    if (b == 0)
                        throw std::runtime_error("Division by zero");
                    stack.push_back(a / b);
                    break;
                }
                case OpType::POW:
                {
                    double b = stack.back();
                    stack.pop_back();
                    double a = stack.back();
                    stack.pop_back();
                    stack.push_back(std::pow(a, b));
                    break;
                }
                case OpType::SQRT:
                {
                    double a = stack.back();
                    stack.pop_back();
                    stack.push_back(std::sqrt(a));
                    break;
                }
                case OpType::SIN:
                {
                    double a = stack.back();
                    stack.pop_back();
                    stack.push_back(std::sin(a));
                    break;
                }
                case OpType::COS:
                {
                    double a = stack.back();
                    stack.pop_back();
                    stack.push_back(std::cos(a));
                    break;
                }
                }
            }
            return stack.back();
        };
    }
};

void replaceAll(std::string &str, const std::string &from, const std::string &to)
{
    if (from.empty())
        return;
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}