#include "shared/test.hpp"

#include "shared/observer.hpp"

namespace shared {

using boost::posix_time::microsec_clock;

test_result test(test_parameters p) {
    auto start = microsec_clock::local_time();

    std::vector<shared::subject<>> subjects(p.subjects_count);
    std::vector<shared::subject<>::observer> observers;
    for (auto& subject : subjects) {
        for (std::size_t i = 0; i < p.observers_count; i++) {
            observers.emplace_back(subject.observe([]() {}));
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

}  // namespace shared
