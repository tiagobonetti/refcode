#include "live_data.hpp"
#include "token.hpp"

#include <iostream>

int main() {
    live_data<int> v = 5;
    std::cout << v.set(7).value << '\n';
}
