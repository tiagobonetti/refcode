#include "value_token/test.hpp"

#include "value_token/subject.hpp"

namespace valuetoken {

using boost::posix_time::microsec_clock;

test_result test(test_parameters p) {
    auto start = microsec_clock::local_time();

    std::vector<valuetoken::subject<>> subjects(p.subjects_count);

    using token_vector = std::vector<valuetoken::token>;
    std::vector<token_vector> tokens;

    for (auto& subject : subjects) {
        token_vector v;
        for (std::size_t i = 0; i < p.observers_count; i++) {
            v.emplace_back(subject.observe([]() {}));
        }
        tokens.emplace_back(std::move(v));
    }

    auto builded = microsec_clock::local_time();

    for (auto& subject : subjects) {
        subject.notify();
    }

    auto called = microsec_clock::local_time();

    auto tokens_it = tokens.cbegin();
    for (auto& subject : subjects) {
        auto& tokens = *tokens_it++;
        for (auto& t : tokens) {
            subject.remove_observer(t);
        }
    }

    auto end = microsec_clock::local_time();

    return {(builded - start), (called - builded), (end - called)};
}

}  // namespace valuetoken
