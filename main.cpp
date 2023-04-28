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
#include "./include/erebus.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

// Macros

#define loop while (true)

// Constants

#define EXIT_SUCCESS 0

auto main(i32 argc, char **argv) -> i32
{

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    setvbuf(stdout, nullptr, _IOFBF, 1000);
#endif

    std::cout << "===== Project Ἔρεβος - Simple Math Solver =====\n"
              << "Usage : Write math expression, invalid keyword will trigger Syntax Error!\n\n";

    auto solver = MathSolver();
    loop
    {
        std::string buffer;
        input(buffer, ">> ");

        if (buffer.find("exit") != std::string::npos || (buffer.find("q") != std::string::npos && buffer.size() == 1))
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