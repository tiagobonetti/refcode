#include "observable/token.hpp"
#include "observable/value.hpp"

#include <iostream>

int main() {
    int initial_value = 100;
    observable::value<int> v{initial_value};

    {
        observable::token t;
        t = v.observe([](int value) { std::cout << "value:" << value << '\n'; });

        for (int i : {1, 2, 3, 4, 99}) {
            v.set_value(i);
        }
    }
    v.set_value(0);
}
