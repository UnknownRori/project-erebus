/**
 * @file erebus_internal.hpp
 * @author UnknownRori (68576836+UnknownRori@users.noreply.github.com)
 * @brief This is internal file not to be used on outside, only use "erebus.hpp" instead
 * @version 0.1
 * @date 2023-05-01
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once
#ifndef UNKNOWNRORI_PROJECT_EREBUS_INTERNAL_HPP
#define UNKNOWNRORI_PROJECT_EREBUS_INTERNAL_HPP

#include <iostream>
#include <vector>
#include <stack>
#include <functional>
#include "./erebus.hpp"
#include "./types.hpp"

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

/**
 * @brief Helper function for popping the stack
 *
 * @tparam T
 * @param stack
 * @return T
 */
template <typename T>
static inline auto pop(std::stack<T> &stack) -> T;

static inline auto parse_int(const std::string &__src) -> Result<f64, Rori::Math::ErrorKind>;
static auto parse(const std::vector<Token> &__src) -> Result<std::stack<Token>, Rori::Math::ErrorKind>;
static auto tokenize(const std::string &__src) -> Result<std::vector<Token>, Rori::Math::ErrorKind>;
static auto calculate(std::stack<Token> &__src) -> Rori::Math::ErrorKind;

/**
 * @brief A algorithm helper function
 *
 */
template <typename _InputIterator, typename F>
static inline auto transform(_InputIterator start, _InputIterator end, _InputIterator dst, std::function<void(F)> lambda) -> void;

#endif