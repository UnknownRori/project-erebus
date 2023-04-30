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

#pragma once
#ifndef UNKNOWNRORI_PROJECT_EREBUS_HPP
#define UNKNOWNRORI_PROJECT_EREBUS_HPP

#include <vector>
#include <string>
#include <cmath>

#include "../include/types.hpp"

namespace Rori::Math
{
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
     * @brief Define the type of Error
     *
     */
    enum ErrorKind
    {
        None,
        SyntaxError,
        ParseIntError,
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
    };
}

#endif