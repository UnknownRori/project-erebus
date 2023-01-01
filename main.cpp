/**
 * @file main.cpp
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
#include <bits/stdc++.h>

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
 * @brief If condition true then push value to vector and continue, only work in loop
 */
#define IF_TRUE_PUSH_VEC_N_CONTINUE(VEC, CONDITION, VALUE) \
    if (CONDITION)                                         \
    {                                                      \
        VEC.push_back(VALUE);                              \
        continue;                                          \
    }

/**
 * @brief Resolve Function Token and push it to stack
 */
#define PUSH_RESOLVED_FUNCTION_TOKEN_STACK(STACK, OP, IS_EQUAL, VALUE) \
    if (OP.get_function_type() == IS_EQUAL)                            \
    {                                                                  \
        STACK.push(CREATE_NUMBER_TOKEN(VALUE));                        \
        return ErrorKind::None;                                        \
    }

/**
 * @brief Resolve Operand and push it to stack
 */
#define PUSH_RESOLVE_OP_TOKEN_STACK(STACK, OP, IS_EQUAL, VALUE) \
    if (OP.get_token() == IS_EQUAL)                             \
    {                                                           \
        STACK.push(CREATE_NUMBER_TOKEN(VALUE));                 \
        return ErrorKind::None;                                 \
    }

#define IF_TRUE_LBITSHIFT_OS(OSTREAM, CONDITION, VALUE) \
    if (CONDITION)                                      \
    {                                                   \
        OSTREAM << " '" << VALUE << "' ";               \
        return OSTREAM;                                 \
    }

/**
 * @brief Check if the passed stack is empty, if true it will return IF_TRUE
 */
#define CHECK_IF_EMPTY(STACK, IF_TRUE) \
    if (STACK.empty())                 \
        return IF_TRUE;

#define CREATE_NUMBER_TOKEN(VAL) Token(TokenType::Number, VAL)

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
    CloseParenthesis,
    Function
};

/**
 * @brief Define Type of Function
 */
enum FunctionType
{
    Sin,
    Cos,
    Tan,
    Acos,
    Asin,
    Atan,
    Sqrt,
    Log,
    Floor,
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
 * @brief Helper function to print help
 *
 */
static inline auto print_help() -> void;

/**
 * @brief Class for representing Token
 *
 */
class Token
{
private:
    TokenType m_type;
    FunctionType m_func_type;
    i32 m_precedence;
    f64 m_value;
    bool m_is_left_associativity;

public:
    Token() = default;
    Token(TokenType type, f64 value);
    Token(TokenType type);
    Token(TokenType type, FunctionType func_type);
    Token(TokenType type, i32 precedence, bool is_left_associativity);

    const TokenType &get_token() const;
    const FunctionType &get_function_type() const;
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
    std::cout << "===== Project Ἔρεβος - Simple Math Solver =====\n"
              << "Usage : Write math expression, invalid keyword will be ignored!\n\n";

    auto solver = MathSolver();
    loop
    {
        std::string buffer;
        input(buffer, ">> ");

        if (buffer.find("exit") != std::string::npos || buffer.find("q") != std::string::npos)
            break;
        if (buffer.find("help") != std::string::npos)
        {
            print_help();
            continue;
        }

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

static inline auto print_help() -> void
{
    std::cout << "\nSupported Operand\t: '+', '-', '*', '/', '^'\n"
              << "Supported Function\t: 'sin', 'cos', 'tan', 'acos', 'asin', 'atan', 'sqrt', 'log', 'floor'\n"
              << "example\t: sin(4*(2+8)^2) it will resulted -0.8509193596\n\n";
}

// Token Class

Token::Token(TokenType type, f64 value)
    : m_type(type), m_value(value) {}

Token::Token(TokenType type) : m_type(type) {}

Token::Token(TokenType type, FunctionType func_type) : m_type(type), m_func_type(func_type) {}

Token::Token(TokenType type, i32 precedence, bool is_left_associativity)
    : m_type(type), m_precedence(precedence), m_is_left_associativity(is_left_associativity) {}

const TokenType &Token::get_token() const { return this->m_type; }
const FunctionType &Token::get_function_type() const { return this->m_func_type; }
const f64 &Token::get_value() const { return this->m_value; }
const i32 &Token::get_precedence() const { return this->m_precedence; }
const bool &Token::is_left_associative() const { return this->m_is_left_associativity; }

std::ostream &operator<<(std::ostream &os, const Token &token)
{
    if (token.m_type == TokenType::Function)
    {
        IF_TRUE_LBITSHIFT_OS(os, token.m_func_type == FunctionType::Sin, "sin");
        IF_TRUE_LBITSHIFT_OS(os, token.m_func_type == FunctionType::Cos, "cos");
        IF_TRUE_LBITSHIFT_OS(os, token.m_func_type == FunctionType::Tan, "tan");
        IF_TRUE_LBITSHIFT_OS(os, token.m_func_type == FunctionType::Asin, "Asin");
        IF_TRUE_LBITSHIFT_OS(os, token.m_func_type == FunctionType::Acos, "Acos");
        IF_TRUE_LBITSHIFT_OS(os, token.m_func_type == FunctionType::Atan, "Atan");
        IF_TRUE_LBITSHIFT_OS(os, token.m_func_type == FunctionType::Sqrt, "Sqrt");
        IF_TRUE_LBITSHIFT_OS(os, token.m_func_type == FunctionType::Log, "Log");
        IF_TRUE_LBITSHIFT_OS(os, token.m_func_type == FunctionType::Floor, "Floor");
    }

    IF_TRUE_LBITSHIFT_OS(os, token.m_type == TokenType::Plus, "+");
    IF_TRUE_LBITSHIFT_OS(os, token.m_type == TokenType::Subtract, "-");
    IF_TRUE_LBITSHIFT_OS(os, token.m_type == TokenType::Multiply, "*");
    IF_TRUE_LBITSHIFT_OS(os, token.m_type == TokenType::Divide, "/");
    IF_TRUE_LBITSHIFT_OS(os, token.m_type == TokenType::PowerOperator, "^");
    IF_TRUE_LBITSHIFT_OS(os, token.m_type == TokenType::OpenParenthesis, "(");
    IF_TRUE_LBITSHIFT_OS(os, token.m_type == TokenType::CloseParenthesis, ")");

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

    if (res.size() != 1)
        return {-1, ErrorKind::SyntaxError};

    return {res.top().get_value(), ErrorKind::None};
}

auto MathSolver::calculate(std::stack<Token> &__src) -> ErrorKind
{
    CHECK_IF_EMPTY(__src, ErrorKind::SyntaxError);
    auto op1 = pop(__src);

    CHECK_IF_EMPTY(__src, ErrorKind::SyntaxError);
    if (__src.top().get_token() != TokenType::Number)
        this->calculate(__src);

    CHECK_IF_EMPTY(__src, ErrorKind::SyntaxError);
    auto op2 = pop(__src);

    if (op1.get_token() == TokenType::Function)
    {
        PUSH_RESOLVED_FUNCTION_TOKEN_STACK(__src, op1, FunctionType::Sin, std::sin(op2.get_value()));
        PUSH_RESOLVED_FUNCTION_TOKEN_STACK(__src, op1, FunctionType::Cos, std::cos(op2.get_value()));
        PUSH_RESOLVED_FUNCTION_TOKEN_STACK(__src, op1, FunctionType::Tan, std::tan(op2.get_value()));
        PUSH_RESOLVED_FUNCTION_TOKEN_STACK(__src, op1, FunctionType::Acos, std::acos(op2.get_value()));
        PUSH_RESOLVED_FUNCTION_TOKEN_STACK(__src, op1, FunctionType::Asin, std::asin(op2.get_value()));
        PUSH_RESOLVED_FUNCTION_TOKEN_STACK(__src, op1, FunctionType::Atan, std::atan(op2.get_value()));
        PUSH_RESOLVED_FUNCTION_TOKEN_STACK(__src, op1, FunctionType::Sqrt, std::sqrt(op2.get_value()));
        PUSH_RESOLVED_FUNCTION_TOKEN_STACK(__src, op1, FunctionType::Log, std::log(op2.get_value()));
        PUSH_RESOLVED_FUNCTION_TOKEN_STACK(__src, op1, FunctionType::Floor, std::floor(op2.get_value()));

        return ErrorKind::SyntaxError;
    }

    CHECK_IF_EMPTY(__src, ErrorKind::SyntaxError);
    if (__src.top().get_token() != TokenType::Number)
        this->calculate(__src);

    CHECK_IF_EMPTY(__src, ErrorKind::SyntaxError);
    auto op3 = pop(__src);

    PUSH_RESOLVE_OP_TOKEN_STACK(__src, op1, TokenType::Plus, op3.get_value() + op2.get_value());
    PUSH_RESOLVE_OP_TOKEN_STACK(__src, op1, TokenType::Subtract, op3.get_value() - op2.get_value());
    PUSH_RESOLVE_OP_TOKEN_STACK(__src, op1, TokenType::Multiply, op3.get_value() * op2.get_value());
    PUSH_RESOLVE_OP_TOKEN_STACK(__src, op1, TokenType::Divide, op3.get_value() / op2.get_value());
    PUSH_RESOLVE_OP_TOKEN_STACK(__src, op1, TokenType::PowerOperator, std::pow(op3.get_value(), op2.get_value()));

    return ErrorKind::None;
}

auto MathSolver::tokenize(const std::string &__src) -> Result<std::vector<Token>, ErrorKind>
{
    std::regex my_regex("([a-zA-Z]+)|\\(|\\)|[-]?((\\d+\\.?\\d*)|(\\.\\d+))|\\*|\\^|\\-|\\+|\\/");
    auto math_begin = std::sregex_iterator(__src.begin(), __src.end(), my_regex);
    auto math_end = std::sregex_iterator();

    std::vector<Token> tokens;
    for (auto i = math_begin; i != math_end; ++i)
    {
        auto token = (*i).str();

        auto [num, err] = this->parse_int(token);
        if (err != ErrorKind::ParseIntError)
        {
            tokens.push_back(CREATE_NUMBER_TOKEN(num));
            continue;
        }

        std::transform(token.begin(), token.end(), token.begin(), ::tolower);

        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "+", Token(TokenType::Plus, 1, true));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "-", Token(TokenType::Subtract, 1, true));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "*", Token(TokenType::Multiply, 2, true));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "/", Token(TokenType::Divide, 2, true));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "^", Token(TokenType::PowerOperator, 3, false));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "(", Token(TokenType::OpenParenthesis));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == ")", Token(TokenType::CloseParenthesis));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "sin", Token(TokenType::Function, FunctionType::Sin));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "cos", Token(TokenType::Function, FunctionType::Cos));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "tan", Token(TokenType::Function, FunctionType::Tan));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "acos", Token(TokenType::Function, FunctionType::Acos));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "asin", Token(TokenType::Function, FunctionType::Asin));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "atan", Token(TokenType::Function, FunctionType::Atan));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "sqrt", Token(TokenType::Function, FunctionType::Sqrt));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "log", Token(TokenType::Function, FunctionType::Log));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "floor", Token(TokenType::Function, FunctionType::Floor));

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