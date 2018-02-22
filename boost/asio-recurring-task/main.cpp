#include <boost/asio.hpp>

#include <iostream>
#include <memory>

template <typename T>
class recurring_task : public std::enable_shared_from_this<recurring_task<T>> {
   public:
    recurring_task(boost::asio::io_service& io,
                   boost::posix_time::time_duration repeat_duration,
                   T&& callback)
        : std::enable_shared_from_this<recurring_task>(),
          _timer(io),
          _repeat_duration(repeat_duration),
          _callback(std::forward<T>(callback)) {}

    void async_execute_from_now(boost::posix_time::time_duration t) {
        auto self = std::enable_shared_from_this<recurring_task<T>>::shared_from_this();
        _timer.expires_from_now(t);
        _timer.async_wait([self](const boost::system::error_code& ec) {
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

   private:
    boost::asio::deadline_timer _timer;
    boost::posix_time::time_duration _repeat_duration;
    T _callback;
    bool _canceled = false;
};

template <typename T>
void make_recurring_task(boost::asio::io_service& io,
                         boost::posix_time::time_duration start_duration,
                         boost::posix_time::time_duration repeat_duration,
                         T&& callback) {
    auto task = std::make_shared<recurring_task<T>>(io,
                                                    repeat_duration,
                                                    std::forward<T>(callback));
    task->async_execute_from_now(start_duration);
}

int main(int argc, char* argv[]) {
    boost::asio::io_service io;

    struct task_callback {
        int count = 10;
        void operator()(bool& stop) {
            std::cout << "recurring functor! count=" << count << '\n';
            if (!count--) {
                stop = true;
            }
        }
    };

    make_recurring_task(io,
                        boost::posix_time::milliseconds(1),
                        boost::posix_time::milliseconds(100),
                        task_callback{});

    io.run();
    return 0;
}
