/**
 * @file erebus.hpp
 * @author UnknownRori (68576836+UnknownRori@users.noreply.github.com)
 * @brief Exposed API of project-erebus
 * @version 0.1
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef UNKNOWNRORI_PROJECT_EREBUS_HPP
#define UNKNOWNRORI_PROJECT_EREBUS

// #include <iostream>
#include <stack>
#include <vector>
#include <tuple>
#include <string>
#include <cmath>
#include <functional>

#include "../include/types.hpp"

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
    Modulo,
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
auto input(std::string &__dst, const char *__msg) -> void;

/**
 * @brief Helper function to print help
 *
 */
auto print_help() -> void;

/**
 * @brief A algorithm helper function
 *
 */
template <typename _InputIterator, typename F>
static inline auto transform(_InputIterator start, _InputIterator end, _InputIterator dst, std::function<void(F)> lambda) -> void;

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

#endif