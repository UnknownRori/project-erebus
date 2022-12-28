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
#include <tuple>
#include <string>
#include <cmath>
#include <regex>

// Constants

#define EXIT_SUCCESS 0

// Custom types

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

template <typename T, typename Y>
using Result = std::tuple<T, Y>;

// Macros

#define loop while (true)

/**
 * @brief Check if the passed stack is empty, if true it will return IF_TRUE
 */
#define CHECK_IF_EMPTY(STACK, IF_TRUE) \
    if (STACK.empty())                 \
        return IF_TRUE;

// Enums

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
    PowerOperator,
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

// Helper Functions

/**
 * @brief Helper function for popping the stack
 *
 * @tparam T
 * @param stack
 * @return T
 */
template <typename T>
inline auto pop(std::stack<T> &stack) -> T;

/**
 * @brief Helper function for getting user input
 *
 * @param __dst
 * @param __msg
 */
static inline auto input(std::string &__dst, const char *__msg) -> void;

/**
 * @brief Class for representing Token
 *
 */
class Token
{
private:
    TokenType m_type;
    i32 m_precedence;
    f64 m_value;
    bool m_is_left_associativity;

public:
    Token() = default;
    Token(TokenType type, f64 value);
    Token(TokenType type);
    Token(TokenType type, i32 precedence, bool is_left_associativity);

    const TokenType &get_token() const;
    const f64 &get_value() const;
    const i32 &get_precedence() const;
    const bool &is_left_associative() const;

    friend std::ostream &operator<<(std::ostream &os, const Token &token);
};

class MathSolver
{
public:
    MathSolver() = default;

    /**
     * @brief Evaluate Math Expressions
     *
     * @param __src
     * @return Result<f64, ErrorKind>
     */
    auto evaluate(const std::string &__src) -> Result<f64, ErrorKind>;

private:
    /**
     * @brief Evaluating Shunting Yard Algorithm AST using recursive approach
     *
     * @param __src
     * @return ErrorKind
     */
    auto calculate(std::stack<Token> &__src) -> ErrorKind;

    /**
     * @brief Split the string into tokens
     *
     * @param __src
     * @return Result<std::vector<std::Token>, ErrorKind>
     */
    auto tokenize(const std::string &__src) -> Result<std::vector<Token>, ErrorKind>;

    /**
     * @brief Convert tokenized string into tokens array using Shunting yard algorithm
     *
     * @param __src
     * @param __dst
     * @return const Result<std::stack<Token>, ErrorKind>
     */
    auto parse(const std::vector<Token> &__src) -> Result<std::stack<Token>, ErrorKind>;

    /**
     * @brief Parse string into a f64
     *
     * @param __src
     * @return Result<f64, ErrorKind>
     */
    auto parse_int(const std::string &__src) -> Result<f64, ErrorKind>;
};

auto main(i32 argc, char **argv) -> i32
{
    std::cout << "===== Simple Math Solver =====\n"
              << "Usage : every expression should be seperated by spaces!\n\n";

    auto solver = MathSolver();
    loop
    {
        std::string buffer;
        input(buffer, ">> ");

        if (buffer.find("exit") != std::string::npos)
            break;

        auto [result, err] = solver.evaluate(buffer);

        if (err == ErrorKind::SyntaxError)
            std::cout << "Error: Syntax Error\n\n";
        else if (err == ErrorKind::ParseIntError)
            std::cout << "Error: Failed to parse integer value\n\n";
        else
            std::cout << "Result\t: " << result << "\n\n";
    }

    std::cout << "\n===Thank you for using this tool!===" << std::endl;

    return EXIT_SUCCESS;
}

// Functions and Classes Definition

template <typename T>
inline auto pop(std::stack<T> &stack) -> T
{
    auto temp = stack.top();
    stack.pop();
    return temp;
}

static inline auto input(std::string &__dst, const char *__msg) -> void
{
    std::cout << __msg;
    std::getline(std::cin, __dst);
}

// Token Class

Token::Token(TokenType type, f64 value)
    : m_type(type), m_value(value) {}

Token::Token(TokenType type) : m_type(type) {}

Token::Token(TokenType type, i32 precedence, bool is_left_associativity)
    : m_type(type), m_precedence(precedence), m_is_left_associativity(is_left_associativity) {}

const TokenType &Token::get_token() const { return this->m_type; }
const f64 &Token::get_value() const { return this->m_value; }
const i32 &Token::get_precedence() const { return this->m_precedence; }
const bool &Token::is_left_associative() const { return this->m_is_left_associativity; }

std::ostream &operator<<(std::ostream &os, const Token &token)
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
    else if (token.m_type == TokenType::PowerOperator)
        os << " '^' ";
    else if (token.m_type == TokenType::OpenParenthesis)
        os << " '(' ";
    else if (token.m_type == TokenType::CloseParenthesis)
        os << " ')' ";

    return os;
}

// Math Solver Class

auto MathSolver::evaluate(const std::string &__src) -> Result<f64, ErrorKind>
{
    auto [tokens, err] = this->tokenize(__src);

    if (err != ErrorKind::None)
        return {-1, err};

    auto [res, err2] = this->parse(tokens);

    if (err2 != ErrorKind::None)
        return {-1, err2};

    if (res.size() == 1 && res.top().get_token() == TokenType::Number)
        return {pop(res).get_value(), ErrorKind::None};

    auto err3 = this->calculate(res);

    if (err3 != ErrorKind::None)
        return {-1, err3};

    return {res.top().get_value(), ErrorKind::None};
}

auto MathSolver::calculate(std::stack<Token> &__src) -> ErrorKind
{
    CHECK_IF_EMPTY(__src, ErrorKind::SyntaxError);
    auto op1 = pop(__src);

    CHECK_IF_EMPTY(__src, ErrorKind::SyntaxError);
    if (__src.top().get_token() != TokenType::Number)
        this->calculate(__src);

    auto op2 = pop(__src);

    CHECK_IF_EMPTY(__src, ErrorKind::SyntaxError);
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

    case TokenType::PowerOperator:
        __src.push(Token(TokenType::Number, std::pow(op3.get_value(), op2.get_value())));
        break;

    default:
        break;
    }

    return ErrorKind::None;
}

auto MathSolver::tokenize(const std::string &__src) -> Result<std::vector<Token>, ErrorKind>
{
    std::regex my_regex("([a-zA-Z]+)|\\(|\\)|(-?[0-9]+\\.?[0-9]?)|\\*|\\^|\\-|\\+|\\/");
    auto math_begin = std::sregex_iterator(__src.begin(), __src.end(), my_regex);
    auto math_end = std::sregex_iterator();

    std::vector<Token> tokens;
    for (auto i = math_begin; i != math_end; ++i)
    {
        auto token = (*i).str();

        auto [num, err] = this->parse_int(token);
        if (err != ErrorKind::ParseIntError)
        {
            tokens.push_back(Token(TokenType::Number, num));
            continue;
        }

        if (token == "+")
            tokens.push_back(Token(TokenType::Plus, 1, true));
        else if (token == "-")
            tokens.push_back(Token(TokenType::Subtract, 1, true));
        else if (token == "*")
            tokens.push_back(Token(TokenType::Multiply, 2, true));
        else if (token == "/")
            tokens.push_back(Token(TokenType::Divide, 2, true));
        else if (token == "^")
            tokens.push_back(Token(TokenType::PowerOperator, 3, false));
        else if (token == "(")
            tokens.push_back(Token(TokenType::OpenParenthesis));
        else if (token == ")")
            tokens.push_back(Token(TokenType::CloseParenthesis));
        else
            return {tokens, ErrorKind::SyntaxError};
    }

    return {tokens, ErrorKind::None};
}

auto MathSolver::parse(const std::vector<Token> &__src) -> Result<std::stack<Token>, ErrorKind>
{
    std::stack<Token> operator_stack;
    std::stack<Token> output;
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
                    output.push(pop(operator_stack));
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
            bool is_top_operator_stack_openparenthesis = operator_stack.top().get_token() != TokenType::OpenParenthesis;
            bool is_token_less_than_operator_stack_top = token.get_precedence() < operator_stack.top().get_precedence();
            bool is_lf_associative_and_equal_precedence = token.get_precedence() == operator_stack.top().get_precedence() && token.is_left_associative();

            if (is_top_operator_stack_openparenthesis && (is_token_less_than_operator_stack_top || is_lf_associative_and_equal_precedence))
                output.push(pop(operator_stack));
            else
                break;
        }

        operator_stack.push(token);
    }

    while (!operator_stack.empty())
        output.push(pop(operator_stack));

    if (parenthesis_count != 0)
        return {output, ErrorKind::SyntaxError};

    return {output, ErrorKind::None};
}

auto MathSolver::parse_int(const std::string &__src) -> Result<f64, ErrorKind>
{
    try
    {
        f64 result = std::stof(__src);
        return {result, ErrorKind::None};
    }
    catch (std::invalid_argument &err)
    {
        return {-1, ErrorKind::ParseIntError};
    }
}