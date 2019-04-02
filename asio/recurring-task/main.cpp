
#include <asio.hpp>

#include <chrono>
#include <iostream>
#include <memory>

template <typename T, typename Clock = std::chrono::steady_clock>
class recurring_task : public std::enable_shared_from_this<recurring_task<T>> {
   public:
    using duration = typename Clock::duration;
    using timer = typename asio::basic_waitable_timer<Clock>;

    recurring_task(asio::io_context& ioc, duration repeat_duration, T&& callback)
        : std::enable_shared_from_this<recurring_task>(),
          _timer(ioc),
          _repeat_duration(repeat_duration),
          _callback(std::forward<T>(callback)) {}

   private:
    void async_execute_from_now(duration t) {
        auto self = std::enable_shared_from_this<recurring_task<T>>::shared_from_this();
        _timer.expires_from_now(t);
        _timer.async_wait([self](const std::error_code& ec) {
            if (ec) {
                // timer object not alive anymore -> do not use 'this'
                std::cout << "timer ec: " << ec.message() << std::endl;
                return;
            }

            self->_callback(static_cast<bool&>(self->_canceled));

            if (!self->_canceled) {
                self->async_execute_from_now(self->_repeat_duration);
            }
        });
    }

    template <typename T_, typename Clock_>
    friend void make_recurring_task(asio::io_service& io,
                                    typename Clock_::duration start_duration,
                                    typename Clock_::duration repeat_duration,
                                    T_&& callback);

    timer _timer;
    duration _repeat_duration;
    T _callback;
    bool _canceled = false;
};

template <typename T, typename Clock = std::chrono::steady_clock>
void make_recurring_task(asio::io_service& io,
                         typename Clock::duration start_duration,
                         typename Clock::duration repeat_duration,
                         T&& callback) {
    using Task = recurring_task<T, Clock>;
    auto task = std::make_shared<Task>(io, repeat_duration, std::forward<T>(callback));
    task->async_execute_from_now(start_duration);
}

int main() {
    using namespace std::literals;
    asio::io_context io;

    make_recurring_task(io, 500ms, 1s, [count = 0](bool) mutable {
        std::cout << "recurring lambda! count=" << count++ << '\n';
    });

    io.run();
    return 0;
}
