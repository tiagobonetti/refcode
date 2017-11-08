#include <iostream>
#include <functional>

int main() {
    std::function<void(void)> f = [&]() {
        f = [&]() {
            f = [&]() {
                f = nullptr;
                std::cout << "Third lambda\n";
            };
            std::cout << "Second lambda\n";
        };
        std::cout << "Fist lambda\n";
    };

    f();
    f();
    f();
    if (f == nullptr) {
        std::cout << "f = nullptr\n";
    }
}
