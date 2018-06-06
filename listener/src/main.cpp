#include <iostream>
#include <string>

#include "auto_token/test.hpp"
#include "shared/test.hpp"
#include "unique/test.hpp"
#include "value_token/test.hpp"

#include "timeval.hpp"

void show_results(std::string header, test_parameters p, test_result r) {
    std::stringstream ss;
    auto call_dt = r.call.total_nanoseconds() /
                   static_cast<int>(p.subjects_count * p.observers_count);

    ss << header << '\n'
       << "  Build - " << r.build << '\n'
       << "  Call  - " << r.call << " (" << call_dt << "ns/call)" << '\n'
       << "  Clean - " << r.clean << '\n';
    std::cout << ss.str();
}

void test_unique(test_parameters p) {
    auto r = unique::test(p);
    show_results(" Unique_ptr Observer", p, r);
}

void test_shared(test_parameters p) {
    auto r = shared::test(p);
    show_results(" Shared_ptr Observer", p, r);
}

void test_valuetoken(test_parameters p) {
    auto r = valuetoken::test(p);
    show_results(" Simple Value Token", p, r);
}

void test_autotoken(test_parameters p) {
    auto r = autotoken::test(p);
    show_results(" Automatic Removal Token", p, r);
}

void test(std::size_t subjects_count, std::size_t observers_count) {
    std::cout << "Test parameters: " << subjects_count << " subjects, " << observers_count << " observers\n";
    test_unique({subjects_count, observers_count});
    test_shared({subjects_count, observers_count});
    test_valuetoken({subjects_count, observers_count});
    test_autotoken({subjects_count, observers_count});
    std::cout << '\n';
}

int main() {
    test(1000000, 1);
    test(1, 1000000);
    test(1000, 1000);
}
