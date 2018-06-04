#include "observable/auto_token.hpp"
#include "observable/value.hpp"
#include "token.hpp"

#include <iostream>

int main() {
    int initial_value = 100;
    observable::value<int> v = initial_value;

    {
        auto t = v.observe([](int value) { std::cout << "value:" << value << '\n'; });

        observable::auto_token at;
        at._removable = &v;
        at._token = t;

        for (int i : {1, 2, 3, 4, 99}) {
            v.set_value(i);
        }
    }
    v.set_value(0);
}
