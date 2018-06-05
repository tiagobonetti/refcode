#include "auto_token/test.hpp"

#include "auto_token/subject.hpp"

namespace autotoken {

using boost::posix_time::microsec_clock;

test_result test(test_parameters p) {
    auto start = microsec_clock::local_time();

    // make subjects
    std::vector<subject<>> subjects(p.subjects_count);
    // observe all subjects
    std::vector<token> tokens;
    for (auto& subject : subjects) {
        for (std::size_t i = 0; i < p.observers_count; i++) {
            tokens.emplace_back(subject.observe([]() {}));
        }
    }
    auto builded = microsec_clock::local_time();

    // call subjects
    for (auto& subject : subjects) {
        subject.notify();
    }
    auto called = microsec_clock::local_time();

    // destroy tokens and subjects
    tokens.clear();
    subjects.clear();
    auto end = microsec_clock::local_time();

    return {(builded - start), (called - builded), (end - called)};
}

}  // namespace autotoken