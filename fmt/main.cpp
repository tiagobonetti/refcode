#include <fmt/format.h>
#include <iostream>
#include <sstream>

void print_scaled(double value) {
    value = (value == 0.0) ? 0.0 : std::clamp(value, 0.0, 100.0);
    std::cout << fmt::format("read value {:f}\n", value);

    auto scale = value / 100.0;
    std::cout << fmt::format("scale is   {:f}\n", scale);

    auto max = std::numeric_limits<std::uint16_t>::max();
    auto scaled = max * scale;
    std::cout << fmt::format("scaled  std::uint16_t {0:.0f} (with decimals: {0:f})\n", scaled);

    auto rounded = static_cast<std::uint16_t>(std::lround(scaled));
    std::cout << fmt::format("rounded std::uint16_t {0:d} (hex: {0:x})\n", rounded);
}

int main() {
    std::string tmp;
    while (std::getline(std::cin, tmp)) {
        if (tmp == "f") {
            break;
        }
        std::stringstream ss(tmp);
        double number;
        ss >> number;

        print_scaled(number);
    }
    return 0;
}
