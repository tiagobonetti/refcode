#include "unique/test.hpp"

#include "unique/subject.hpp"

namespace unique {

using boost::posix_time::microsec_clock;

test_result test(test_parameters p) {
    auto start = microsec_clock::local_time();

    std::vector<unique::subject<>> subjects(p.subjects_count);
    std::vector<std::unique_ptr<const unique::observer<>>> observers;

    for (auto& subject : subjects) {
        for (std::size_t i = 0; i < p.observers_count; i++) {
            auto observer = subject.attach([]() { /* noop */ });
            observers.push_back(std::move(observer));
        }
    }

    auto builded = microsec_clock::local_time();

    for (auto& subject : subjects) {
        subject.notify();
    }

    auto called = microsec_clock::local_time();

    observers.clear();
    subjects.clear();

    auto end = microsec_clock::local_time();

    return {(builded - start), (called - builded), (end - called)};
}

}  // namespace unique
