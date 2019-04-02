#include <asio.hpp>

#include <chrono>
#include <iostream>
#include <thread>

int main() {
    using namespace std::literals;

    auto ioc = asio::io_context{};

    auto timer = asio::steady_timer{ioc, 2s};
    timer.async_wait([](const std::error_code& ec) {
        if (ec) {
            return;
        }
        std::cout << "Timer expired!\n";
    });

    auto t = std::thread{[&]() {
        auto guard = asio::executor_work_guard{ioc.get_executor()};
        ioc.run();
    }};

    std::this_thread::sleep_for(5s);
    ioc.stop();
    t.join();

    std::cout << "Finished!\n";
    return 0;
}
