#include "unique/test.hpp"

#include "unique/subject.hpp"
#include "unique/subject2.hpp"

namespace unique {

using boost::posix_time::microsec_clock;

test_result test(test_parameters p) {
    unsigned call_count = 0;

    auto start = microsec_clock::local_time();

    std::vector<subject<>> subjects(p.subjects_count);
    std::vector<std::unique_ptr<const observer<>>> observers;

    for (auto& subject : subjects) {
        for (std::size_t i = 0; i < p.observers_count; i++) {
            auto observer = subject.attach([&]() { call_count++; });
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

    assert(call_count == p.subjects_count * p.observers_count);

    return {(builded - start), (called - builded), (end - called)};
}

test_result test2(test_parameters p) {
    unsigned call_count = 0;

    auto start = microsec_clock::local_time();

    std::vector<subject2<>> subjects(p.subjects_count);
    std::vector<unique::subject2<>::observer_type> observers;

    for (auto& subject : subjects) {
        for (std::size_t i = 0; i < p.observers_count; i++) {
            auto observer = subject.observe([&]() { call_count++; });
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

    assert(call_count == p.subjects_count * p.observers_count);

    return {(builded - start), (called - builded), (end - called)};
}

}  // namespace unique
