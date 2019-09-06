#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    auto m = [](auto v) {
        std::nth_element(v.begin(), v.begin() + v.size() / 2, v.end());
        std::cout << "The median is " << v[v.size() / 2] << '\n';
        std::cout << " v= {";
        for (auto e : v) {
            std::cout << ' ' << e << ',';
        }
        std::cout << "}\n";
    };

    m(std::vector{5, 6, 4, 3, 2, 6, 7, 9, 3});
    m(std::vector{5, 6, 4, 3, 2, 6, 7, 9, 3});

    m(std::vector{5, 6, 4, 3, 2, 6, 7, 9, 3});


}
