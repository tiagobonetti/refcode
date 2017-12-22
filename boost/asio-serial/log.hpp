#pragma once
#include <iomanip>
#include <ios>
#include <sstream>

#define TRACE(msg)                                                                                      \
    do {                                                                                                \
        std::stringstream stringstream__;                                                               \
        stringstream__ << "\e[32m" << std::setfill('0') << std::setw(4) << __LINE__ << "\e[0m: "        \
                       << "\e[34m" << std::setfill(' ') << std::setw(30) << __FUNCTION__ << "\e[0m - "; \
        (stringstream__ msg);                                                                           \
        stringstream__ << '\n';                                                                         \
        std::cout << stringstream__.str() << std::flush;                                                \
    } while (0)
