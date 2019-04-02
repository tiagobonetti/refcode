#include <fmt/format.h>
#include <iostream>
#include <sstream>

template <typename T>
T get_number(std::string const& text) {
    fmt::print("{}\n", text);
    std::string tmp;
    while (std::getline(std::cin, tmp)) {
        std::stringstream ss(tmp);
        auto value = T{0};
        ss >> value;

        if (ss.fail()) {
            auto what = fmt::format("Ivalid conversion from input '{}'!", tmp);
            throw std::runtime_error{what};
        }

        return value;
    }
    throw std::runtime_error{"Impossibru!"};
}

int main() {
    for (;;) {
        auto division = get_number<int>("Enter divisor:");
        auto duty_cycle = get_number<double>("Enter duty cycle:");
        fmt::print("division={} duty_cycle={}\n", division, duty_cycle);

        {
            fmt::print("Rounded:\n");
            auto high = static_cast<int>(std::round(division * duty_cycle));
            auto low = static_cast<int>(division - high);

            auto real = static_cast<double>(high) / division;
            auto error = std::abs(duty_cycle - real);

            fmt::print(" high={} low={}\n", high, low);
            fmt::print(" duty cylcle: real={} error={:3f}\n", real, error);

            auto min = (0.5 / division);
            auto max = 1 - min;
            fmt::print(" [min={}, max={})\n", min, max);
        }
        {
            fmt::print("Trucated:\n");
            auto high = static_cast<int>(std::trunc(division * duty_cycle));
            auto low = static_cast<int>(division - high);

            auto real = static_cast<double>(high) / division;
            auto error = std::abs(duty_cycle - real);

            fmt::print(" high={} low={}\n", high, low);
            fmt::print(" duty cylcle: real={} error={:3f}\n", real, error);

            auto min = (1.0 / division);
            auto max = 1.0;
            fmt::print(" [min={}, max={})\n", min, max);
        }
        {
            fmt::print("Ceil:\n");
            auto high = static_cast<int>(std::ceil(division * duty_cycle));
            auto low = static_cast<int>(division - high);

            auto real = static_cast<double>(high) / division;
            auto error = std::abs(duty_cycle - real);

            fmt::print(" high={} low={}\n", high, low);
            fmt::print(" duty cylcle: real={} error={:3f}\n", real, error);

            auto min = 0.0;
            auto max = 1.0 - (1.0 / division);
            fmt::print("(min={}, max={:2f}]\n", min, max);
        }
    }
    return 0;
}
