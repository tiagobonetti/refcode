#include <fmt/format.h>
#include <regex>
#include <set>
#include <string>

template <class F>
auto next_unique_name(const std::string& name, const F& is_unique) {
    const auto re = std::regex("(.*?)\\s*(?:\\((\\d+)\\))?");
    std::smatch match;
    if (!std::regex_match(name, match, re)) {
        throw std::logic_error("this regular expression should be able to match anything");
    }

    const auto make_candidate = [basename = match.str(1)](int i) {
        return fmt::format("{} ({})", basename, i);
    };

    const auto index = match.str(2);
    const auto start = index.empty() ? 2 : std::stoi(index);

    for (int i = start; i <= 999; ++i) {
        if (const auto candidate = make_candidate(i);
            is_unique(candidate)) {
            return candidate;
        }
    }
    throw std::runtime_error("Max unique name index exceeded: " + name);
}

int main() {
    std::set<std::string> existing_names = {"example", "example (2)"};
    auto print_next_unique = [&](std::string const& name) {
        auto is_unique = [&](std::string const& n) { return existing_names.count(n) == 0; };
        auto quote = [](std::string s) { return fmt::format("'{}'", s); };
        try {
            fmt::print("{:20} -> {:20}\n", quote(name), quote(next_unique_name(name, is_unique)));
        } catch (std::exception const& e) {
            fmt::print("{:20} -> {}\n", quote(name), quote(e.what()));
        }
    };
    print_next_unique("example");
    print_next_unique("example ");
    print_next_unique("example  ");
    print_next_unique("example(2)");
    print_next_unique("example (2)");
    print_next_unique("example  (2)");
    print_next_unique("example(6)");
    print_next_unique("example (6)");
    print_next_unique("example   (6)");
    print_next_unique("example (999)");
    print_next_unique("example (1000)");
}
