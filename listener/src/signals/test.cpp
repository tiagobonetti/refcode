#include "auto_token/test.hpp"

#define BOOST_DISABLE_THREADS
#include <boost/signals2.hpp>

namespace signals {

test_result test(test_parameters p) {
    unsigned call_count = 0;

    auto start = now();

    // make signals
    //
    using signal_type = boost::signals2::signal<void()>;
    std::vector<signal_type> signals(p.subjects_count);

    // observe all signals
    std::vector<boost::signals2::scoped_connection> connections;
    for (auto& signal : signals) {
        for (std::size_t i = 0; i < p.observers_count; i++) {
            connections.emplace_back(signal.connect([&]() { call_count++; }));
        }
    }

    auto builded = now();

    // call signals
    for (auto& signal : signals) {
        signal();
    }
    auto called = now();

    // destroy connections and signals
    connections.clear();
    signals.clear();
    auto end = now();

    assert(call_count == p.subjects_count * p.observers_count);

    return {(builded - start), (called - builded), (end - called)};
}

}  // namespace signals
