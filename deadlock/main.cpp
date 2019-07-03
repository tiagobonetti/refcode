#include <iostream>
#include <mutex>
#include <future>

std::recursive_mutex m;

int f() {
    const auto l = std::lock_guard{m};
    std::cout << "f()\n";
    return 1;
}

int g() {
    const auto l = std::lock_guard{m};
    std::cout << "g()\n";
    return std::async(f).get();
}

int main(int argc, const char** argv) {
    const auto result = g();
    std::cout << "g()=" << result << '\n';
}
