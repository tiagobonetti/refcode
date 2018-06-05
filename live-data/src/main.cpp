#include "observable/token.hpp"
#include "observable/value.hpp"
#include "live_data.hpp"

#include <iostream>

void observable_value() {
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

void live_data() {
    int initial_value = 100;
    live::data<int> data{initial_value};

    {
        auto observer = data.observe([](int value) { std::cout << "value:" << value << '\n'; });
        for (int i : {5, 6, 7, 8, 98}) {
            data.set_value(i);
        }
    }
    data.set_value(0);
}
int main() {
    observable_value();
    live_data();
}
