#include "live/value.hpp"
#include "token.hpp"

#include <iostream>

int main() {
    live::value<int> v = 5;
    std::cout << v.set_value(7).get_value() << '\n';
}
