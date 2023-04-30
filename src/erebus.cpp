/**
 * @file erebus.cpp
 * @author UnknownRori (68576836+UnknownRori@users.noreply.github.com)
 * @brief Implementation details of project-erebus math resolver
 * @version 1.0
 * @date 2023-04-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include <functional>
#include <stack>
#include <tuple>
#include "../include/erebus_internal.hpp"
#include "../include/erebus.hpp"
#include "../include/macros.hpp"

// Functions and Classes Definition

static inline auto parse_int(const std::string &__src) -> Result<f64, Rori::Math::ErrorKind>
{
    try
    {
        f64 result = std::stof(__src);
        return {result, Rori::Math::ErrorKind::None};
    }
    catch (std::invalid_argument &err)
    {
        return {-1, Rori::Math::ErrorKind::ParseIntError};
    }
}

template <typename T>
static inline auto pop(std::stack<T> &stack) -> T
{
    auto temp = stack.top();
    stack.pop();
    return temp;
}

auto Rori::Math::input(std::string &__dst, const char *__msg) -> void
{
    std::cout << __msg;
    std::getline(std::cin, __dst);
}

auto Rori::Math::print_help() -> void
{
    std::cout << "\nSupported Operand\t: '+', '-', '*', '/', '^', '%'\n"
              << "Supported Function\t: 'sin', 'cos', 'tan', 'acos', 'asin', 'atan', 'sqrt', 'log', 'floor'\n"
              << "example\t: sin(4*(2+8)^2) it will resulted -0.8509193596\n\n";
}

template <typename _InputIterator, typename F>
static inline auto transform(_InputIterator start, _InputIterator end, _InputIterator dst, std::function<void(F)> lambda) -> void
{
    for (; start != end; start++)
    {
        lambda(*start);
    }
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

auto Rori::Math::MathSolver::evaluate(const std::string &__src) -> Result<f64, Rori::Math::ErrorKind>
{
    auto [tokens, err] = tokenize(__src);

    if (err != Rori::Math::ErrorKind::None)
        return {-1, err};

    auto [res, err2] = parse(tokens);

    if (err2 != Rori::Math::ErrorKind::None)
        return {-1, err2};

    if (res.size() == 1 && res.top().get_token() == TokenType::Number)
        return {pop(res).get_value(), Rori::Math::ErrorKind::None};

    auto err3 = calculate(res);

    if (err3 != Rori::Math::ErrorKind::None)
        return {-1, err3};

    if (res.size() != 1)
        return {-1, Rori::Math::ErrorKind::SyntaxError};

    return {res.top().get_value(), Rori::Math::ErrorKind::None};
}

static auto calculate(std::stack<Token> &__src) -> Rori::Math::ErrorKind
{
    CHECK_IF_EMPTY(__src, Rori::Math::ErrorKind::SyntaxError);
    auto op1 = pop(__src);

    CHECK_IF_EMPTY(__src, Rori::Math::ErrorKind::SyntaxError);
    if (__src.top().get_token() != TokenType::Number)
        calculate(__src);

    CHECK_IF_EMPTY(__src, Rori::Math::ErrorKind::SyntaxError);
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

        return Rori::Math::ErrorKind::SyntaxError;
    }

    CHECK_IF_EMPTY(__src, Rori::Math::ErrorKind::SyntaxError);
    if (__src.top().get_token() != TokenType::Number)
        calculate(__src);

    CHECK_IF_EMPTY(__src, Rori::Math::ErrorKind::SyntaxError);
    auto op3 = pop(__src);

    PUSH_RESOLVE_OP_TOKEN_STACK(__src, op1, TokenType::Plus, op3.get_value() + op2.get_value());
    PUSH_RESOLVE_OP_TOKEN_STACK(__src, op1, TokenType::Subtract, op3.get_value() - op2.get_value());
    PUSH_RESOLVE_OP_TOKEN_STACK(__src, op1, TokenType::Multiply, op3.get_value() * op2.get_value());
    PUSH_RESOLVE_OP_TOKEN_STACK(__src, op1, TokenType::Divide, op3.get_value() / op2.get_value());
    PUSH_RESOLVE_OP_TOKEN_STACK(__src, op1, TokenType::PowerOperator, std::pow(op3.get_value(), op2.get_value()));
    PUSH_RESOLVE_OP_TOKEN_STACK(__src, op1, TokenType::Modulo, std::fmod(op3.get_value(), op2.get_value()));

    return Rori::Math::ErrorKind::None;
}

static auto tokenize(const std::string &__src) -> Result<std::vector<Token>, Rori::Math::ErrorKind>
{
    std::vector<std::string> tokenized;

    std::size_t i = 0;
    while (i < __src.length())
    {
        if (__src[i] == ' ' || __src[i] == ',')
        {
            i++;
            continue;
        }

        if (std::isalpha(__src[i]))
        {
            std::size_t start = i;
            while (std::isalpha(__src[i]))
                i++;

            tokenized.push_back(__src.substr(start, i - start));
            continue;
        }

        if (__src[i] == '(')
        {
            tokenized.push_back(__src.substr(i, 1));
            i++;

            std::size_t start = i;
            if (__src[i] == '-')
            {
                i++;

                PARSE_INT_FROM_STR(__src, start, i);

                tokenized.push_back(__src.substr(start, i - start));
                continue;
            }
            continue;
        }

        if (__src[i] == '-' && i == 0)
        {
            std::size_t start = i;
            i++;

            PARSE_INT_FROM_STR(__src, start, i);

            tokenized.push_back(__src.substr(start, i - start));
            continue;
        }

        if (!std::isdigit(__src[i]))
        {
            tokenized.push_back(__src.substr(i, 1));
            i++;
            continue;
        }

        if (std::isdigit(__src[i]))
        {
            std::size_t start = i;

            PARSE_INT_FROM_STR(__src, start, i);

            tokenized.push_back(__src.substr(start, i - start));
            continue;
        }
        i++;
    }

    std::vector<Token> tokens;
    for (auto token : tokenized)
    {
        auto [num, err] = parse_int(token);
        if (err != Rori::Math::ErrorKind::ParseIntError)
        {
            tokens.push_back(CREATE_NUMBER_TOKEN(num));
            continue;
        }

        transform(token.begin(), token.end(), token.begin(), ::tolower);

        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "+", Token(TokenType::Plus, 1, true));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "-", Token(TokenType::Subtract, 1, true));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "*", Token(TokenType::Multiply, 2, true));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "/", Token(TokenType::Divide, 2, true));
        IF_TRUE_PUSH_VEC_N_CONTINUE(tokens, token == "%", Token(TokenType::Modulo, 2, true));
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

        return {tokens, Rori::Math::ErrorKind::SyntaxError};
    }

    return {tokens, Rori::Math::ErrorKind::None};
}

static auto parse(const std::vector<Token> &__src) -> Result<std::stack<Token>, Rori::Math::ErrorKind>
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
            if (parenthesis_count == 0)
                return {output, Rori::Math::ErrorKind::SyntaxError};

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
        return {output, Rori::Math::ErrorKind::SyntaxError};

    return {output, Rori::Math::ErrorKind::None};
}
