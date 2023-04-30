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
#include <csignal>
#include "./include/erebus.hpp"

#ifdef _WIN32
#include <Windows.h>
#endif

#define loop while (true)
#define THANK_YOU "\n===Thank you for using this tool!===\n"
#define EXIT_SUCCESS 0

auto signal_handler(int) -> void;

auto main(i32 argc, char **argv) -> i32
{
#ifdef _WIN32
    if (!SetConsoleOutputCP(CP_UTF8) && setvbuf(stdout, nullptr, _IOFBF, 1000) != 0)
    {
        std::cout << "[Warning]: Failed to change terminal into UTF-8\n";
    }
#endif

    signal(SIGINT, signal_handler);

    std::cout << "===== Project Ἔρεβος - Simple Math Solver =====\n"
              << "Usage : Write math expression, invalid keyword will trigger Syntax Error!\n\n";

    auto solver = Rori::Math::MathSolver();
    loop
    {
        std::string buffer;
        Rori::Math::input(buffer, ">> ");

        if (buffer.find("exit") != std::string::npos || (buffer.find("q") != std::string::npos && buffer.size() == 1))
            break;
        if (buffer.find("help") != std::string::npos)
        {
            Rori::Math::print_help();
            continue;
        }

        auto [result, err] = solver.evaluate(buffer);

        if (err == Rori::Math::ErrorKind::SyntaxError)
            std::cout << "Error: Syntax Error\n\n";
        else if (err == Rori::Math::ErrorKind::ParseIntError)
            std::cout << "Error: Failed to parse integer value\n\n";
        else
            std::cout << "Result\t: " << result << "\n\n";
    }

    std::cout << THANK_YOU;

    return EXIT_SUCCESS;
}

auto signal_handler(int __signum) -> void
{
    std::cout << THANK_YOU;

    exit(__signum);
}
