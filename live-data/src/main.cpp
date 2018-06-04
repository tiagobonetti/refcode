#include "observable/value.hpp"
#include "token.hpp"

#include <iostream>

int main() {
    observable::value<int> v = 9;
    auto t = v.observe([](int value) { std::cout << "value:" << value << '\n'; });
    v.remove_observer(t);
}
