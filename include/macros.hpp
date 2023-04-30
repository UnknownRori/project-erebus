/**
 * @file macros.hpp
 * @author UnknownRori (68576836+UnknownRori@users.noreply.github.com)
 * @brief header file that include erebus.cpp macros not for external use
 * @version 0.1
 * @date 2023-04-28
 *
 * @copyright Copyright (c) 2023
 *
 */

#pragma once
#ifndef UNKNOWNRORI_MACROS_PROJECT_EREBUS_HPP
#define UNKNOWNRORI_MACROS_PROJECT_EREBUS_HPP

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
        return Rori::Math::ErrorKind::None;                            \
    }

/**
 * @brief Resolve Operand and push it to stack
 */
#define PUSH_RESOLVE_OP_TOKEN_STACK(STACK, OP, IS_EQUAL, VALUE) \
    if (OP.get_token() == IS_EQUAL)                             \
    {                                                           \
        STACK.push(CREATE_NUMBER_TOKEN(VALUE));                 \
        return Rori::Math::ErrorKind::None;                     \
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

#define PARSE_INT_FROM_STR(SRC, START, I)                                 \
    {                                                                     \
        bool is_not_decimal = true;                                       \
        while (std::isdigit(SRC[I]) || (SRC[I] == '.' && is_not_decimal)) \
        {                                                                 \
            if (SRC[I] == '.')                                            \
                is_not_decimal = false;                                   \
            I++;                                                          \
        }                                                                 \
    }

#endif