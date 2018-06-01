#include "token.hpp"

token::value_type token::_count = 0;

std::ostream& operator<<(std::ostream& os, const token& t) {
    return os << "token:" << t._value;
}
