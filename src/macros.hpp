#pragma once

#include <cstdlib>
#include <cstring>
#include <iostream>

inline auto ASSERT(bool condition, const std::string& msg) noexcept
{
    if (!condition) [[unlikely]] {
        std::cerr << msg << std::endl;
        exit(EXIT_FAILURE);
    }
}

inline auto FATAL(const std::string& msg) noexcept
{
    std::cerr << msg << std::endl;
    exit(EXIT_FAILURE);
}
