#include <iostream>
#include <string>

#include "auto_token/test.hpp"
#include "shared/test.hpp"
#include "signals/test.hpp"
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

void test_unique2(test_parameters p) {
    auto r = unique::test2(p);
    show_results(" Unique_ptr Observer (vector container)", p, r);
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

void test_autotoken2(test_parameters p) {
    auto r = autotoken::test2(p);
    show_results(" Automatic Removal Token (vector container)", p, r);
}

void test_signals(test_parameters p) {
    auto r = signals::test(p);
    show_results(" Boost.Signals2", p, r);
}

void test(std::size_t subjects_count, std::size_t observers_count) {
    std::cout << "Test parameters: " << subjects_count << " subjects, " << observers_count << " observers\n";
    test_unique({subjects_count, observers_count});
    test_unique2({subjects_count, observers_count});
    test_shared({subjects_count, observers_count});
    test_valuetoken({subjects_count, observers_count});
    test_autotoken({subjects_count, observers_count});
    test_autotoken2({subjects_count, observers_count});
    test_signals({subjects_count, observers_count});
    std::cout << '\n';
}

int main() {
    test(1000000, 1);
    test(1, 10000);
    test(100, 1000);
}
