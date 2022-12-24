/**
 * @file MathSolver
 * @author UnknownRori (68576836+UnknownRori@users.noreply.github.com)
 * @brief A simple program to solve simple math expressions
 * @version 1.0
 * @date 2022-12-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <iostream>
#include <stack>
#include <vector>
#include <sstream>
#include <tuple>
#include <string>

#define EXIT_SUCCESS 0

/**
 * @brief Definition of custom type
 *
 */
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef double f32;
typedef long double f64;

/**
 * @brief Define Type of Token
 *
 */
enum TokenType
{
    Number,
    Plus,
    Subtract,
    Multiply,
    Divide,
    OpenParenthesis,
    CloseParenthesis
};

/**
 * @brief Define the type of Error
 *
 */
enum ErrorKind
{
    None,
    SyntaxError,
    ParseIntError,
};

/**
 * @brief Helper function for popping the stack
 *
 * @tparam T
 * @param stack
 * @return T
 */
template <typename T>
inline auto pop(std::stack<T> &stack) -> T
{
    auto temp = stack.top();
    stack.pop();
    return temp;
}

/**
 * @brief Class for representing Token
 *
 */
class Token
{
private:
    TokenType m_type;
    i32 m_precedence;
    i64 m_value;

public:
    Token() = default;
    Token(TokenType type, i64 value) : m_type(type), m_value(value) {}
    Token(TokenType type) : m_type(type) {}
    Token(TokenType type, i32 precedence) : m_type(type), m_precedence(precedence) {}

    const TokenType &get_token() const { return this->m_type; }
    const i64 &get_value() const { return this->m_value; }
    const i32 &get_precedence() const { return this->m_precedence; }

    friend std::ostream &operator<<(std::ostream &os, const Token &token)
    {
        if (token.m_type == TokenType::Number)
            os << " '" << token.m_value << "' ";
        else if (token.m_type == TokenType::Plus)
            os << " '+' ";
        else if (token.m_type == TokenType::Subtract)
            os << " '-' ";
        else if (token.m_type == TokenType::Multiply)
            os << " '*' ";
        else if (token.m_type == TokenType::Divide)
            os << " '/' ";
        else if (token.m_type == TokenType::OpenParenthesis)
            os << " '(' ";
        else if (token.m_type == TokenType::CloseParenthesis)
            os << " ')' ";

        return os;
    }
};

class MathSolver
{
public:
    MathSolver() = default;

    /**
     * @brief Evaluate Math Expressions
     *
     * @param __src
     * @return std::tuple<i64, ErrorKind>
     */
    auto evaluate(const std::string &__src) -> std::tuple<i64, ErrorKind>
    {
        auto [tokens, err] = this->tokenize(__src);

        if (err != ErrorKind::None)
            return std::tuple<i64, ErrorKind>(-1, err);

        auto [res, err2] = this->parse(tokens);

        if (err2 != ErrorKind::None)
            return std::tuple<i64, ErrorKind>(-1, err2);

        this->calculate(res);

        return std::tuple<i64, ErrorKind>(res.top().get_value(), ErrorKind::None);
    }

private:
    /**
     * @brief Evaluating Shunting Yard Algorithm AST using recursive approach
     *
     * @param __src
     */
    auto calculate(std::stack<Token> &__src) -> void
    {
        auto op1 = pop(__src);

        if (__src.top().get_token() != TokenType::Number)
            this->calculate(__src);

        auto op2 = pop(__src);

        if (__src.top().get_token() != TokenType::Number)
            this->calculate(__src);

        auto op3 = pop(__src);

        switch (op1.get_token())
        {
        case TokenType::Plus:
            __src.push(Token(TokenType::Number, op3.get_value() + op2.get_value()));
            break;

        case TokenType::Subtract:
            __src.push(Token(TokenType::Number, op3.get_value() - op2.get_value()));
            break;

        case TokenType::Multiply:
            __src.push(Token(TokenType::Number, op3.get_value() * op2.get_value()));
            break;

        case TokenType::Divide:
            __src.push(Token(TokenType::Number, op3.get_value() / op2.get_value()));
            break;

        default:
            break;
        }
    }

    /**
     * @brief Split the string into tokens
     *
     * @param __src
     * @return std::tuple<std::vector<std::Token>, ErrorKind>
     */
    auto tokenize(const std::string &__src) -> std::tuple<std::vector<Token>, ErrorKind>
    {
        std::vector<std::string> result;

        std::stringstream ss(__src);

        std::string temp;
        while (std::getline(ss, temp, ' '))
        {
            if (temp != "")
                result.push_back(temp);
        }

        std::vector<Token> tokens;
        for (const auto &token : result)
        {
            auto [num, err] = this->parse_int(token);
            if (err != ErrorKind::ParseIntError)
            {
                tokens.push_back(Token(TokenType::Number, num));
                continue;
            }

            if (token == "+")
                tokens.push_back(Token(TokenType::Plus, 1));
            else if (token == "-")
                tokens.push_back(Token(TokenType::Subtract, 1));
            else if (token == "*")
                tokens.push_back(Token(TokenType::Multiply, 2));
            else if (token == "/")
                tokens.push_back(Token(TokenType::Divide, 2));
            else if (token == "(")
                tokens.push_back(Token(TokenType::OpenParenthesis));
            else if (token == ")")
                tokens.push_back(Token(TokenType::CloseParenthesis));
            else
                return std::tuple<std::vector<Token>, ErrorKind>(tokens, ErrorKind::SyntaxError);
        }

        return std::tuple<std::vector<Token>, ErrorKind>(tokens, ErrorKind::None);
    }

    /**
     * @brief Convert tokenized string into tokens array using Shunting yard algorithm
     *
     * @param __src
     * @param __dst
     * @return const std::tuple<std::stack<Token>, ErrorKind>
     */
    auto parse(const std::vector<Token> &__src) -> std::tuple<std::stack<Token>, ErrorKind>
    {
        std::stack<Token> operator_stack;
        std::stack<Token> output;

        if (__src.size() < 2)
            return std::tuple<std::stack<Token>, ErrorKind>(output, ErrorKind::SyntaxError);

        i32 parenthesis_count = 0;

        for (auto &token : __src)
        {
            if (token.get_token() == TokenType::Number)
            {
                output.push(token);
                continue;
            }

            if (token.get_token() == TokenType::OpenParenthesis)
            {
                operator_stack.push(token);
                parenthesis_count++;
                continue;
            }

            if (token.get_token() == TokenType::CloseParenthesis)
            {
                parenthesis_count--;
                while (true)
                {
                    if (operator_stack.top().get_token() != TokenType::OpenParenthesis)
                    {

                        output.push(operator_stack.top());
                        operator_stack.pop();
                    }
                    else
                    {
                        operator_stack.pop();
                        break;
                    }
                }
                continue;
            }

            while (!operator_stack.empty())
            {
                if (operator_stack.top().get_token() != TokenType::OpenParenthesis && token.get_precedence() < operator_stack.top().get_precedence())
                {

                    output.push(operator_stack.top());
                    operator_stack.pop();
                }
                else
                {
                    break;
                }
            }

            operator_stack.push(token);
        }

        while (!operator_stack.empty())
        {
            output.push(operator_stack.top());
            operator_stack.pop();
        }

        if (parenthesis_count != 0)
            return std::tuple<std::stack<Token>, ErrorKind>(output, ErrorKind::SyntaxError);

        return std::tuple<std::stack<Token>, ErrorKind>(output, ErrorKind::None);
    }

    /**
     * @brief Parse string into a i64
     *
     * @param __src
     * @return std::tuple<i64, ErrorKind>
     */
    auto parse_int(const std::string &__src) -> std::tuple<i64, ErrorKind>
    {
        try
        {
            i64 result = std::stoll(__src);
            std::tuple<i64, ErrorKind> ret(result, ErrorKind::None);
            return ret;
        }
        catch (std::invalid_argument &err)
        {
            std::tuple<i64, ErrorKind> ret(-1, ErrorKind::ParseIntError);
            return ret;
        }
    }
};

static inline auto input(std::string &__dst, const char *__msg) -> void
{
    std::cout << __msg;
    std::getline(std::cin, __dst);
}

auto main(int argc, char **argv) -> i32
{
    std::string buffer;

    std::cout << "===== Simple Math Solver =====\n";
    std::cout << "Usage : every expression should be seperated by spaces!\n\n";

    while (true)
    {
        input(buffer, ">> ");

        if (buffer.find("exit") != std::string::npos)
            break;

        auto solver = MathSolver();
        auto [result, err] = solver.evaluate(buffer);

        if (err == ErrorKind::SyntaxError)
            std::cout << "Error: Syntax Error\n"
                      << std::endl;
        else if (err == ErrorKind::ParseIntError)
            std::cout << "Error: Failed to parse integer value\n"
                      << std::endl;
        else
            std::cout << "Result\t: " << result << "\n\n";
    }

    std::cout << "\n===Thank you for using this tool!===" << std::endl;

    return EXIT_SUCCESS;
}
