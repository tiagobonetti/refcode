#include <boost/asio.hpp>

#include <ctime>
#include <iostream>
#include <memory>
#include <thread>
#include <utility>
#include <vector>

#if 1

class recurring_task : public std::enable_shared_from_this<recurring_task> {
   public:
    template <typename T>
    static void start(boost::asio::io_service& io,
               boost::posix_time::time_duration start_duration,
               boost::posix_time::time_duration repeat_duration,
               T&& callback) {
        auto task = std::make_shared<recurring_task>(io, repeat_duration);
        task->arm(start_duration, callback);
    }

    void cancel() { _canceled = true; }

    recurring_task(boost::asio::io_service& io,
                   boost::posix_time::time_duration repeat_duration)
        : std::enable_shared_from_this<recurring_task>(),
          _timer(io),
          _repeat_duration(repeat_duration) {
    }

    template <typename T>
    void arm(boost::posix_time::time_duration t, T callback) {
        auto self = shared_from_this();
        _timer.expires_from_now(t);
        _timer.async_wait([self, callback](const boost::system::error_code& ec) {
            if (ec) {
                // timer object not alive anymore -> do not use 'this'
                std::cout << "timer ec: " << ec.message() << std::endl;
                return;
            }

            std::cout << "running callback:\n";
            callback(*self);

            if (!self->_canceled) {
                self->arm(self->_repeat_duration, callback);
            }
        });
    }

    boost::asio::deadline_timer _timer;
    boost::posix_time::time_duration _repeat_duration;
    bool _canceled = false;
};

#endif

template <typename T>
void arm(std::shared_ptr<boost::asio::deadline_timer> timer,
         boost::posix_time::time_duration st,
         boost::posix_time::time_duration rt,
         T callback) {
    timer->expires_from_now(st);
    timer->async_wait(
        [timer, rt, callback](const boost::system::error_code& ec) {
            std::cout << "running lambda\n";

            if (ec) {
                std::cout << "timer ec: " << ec.message() << '\n';
                return;
            }

            arm(timer, rt, rt, callback);
            callback();
        });
}

int main(int argc, char* argv[]) {
    std::cout << "begin\n";

    boost::asio::io_service io;

    auto timer = std::make_shared<boost::asio::deadline_timer>(io);

    int count = 10;
    arm(timer, boost::posix_time::milliseconds(1), boost::posix_time::seconds(1), [timer, &count] {
        std::cout << "count = " << count << '\n';
        if (count-- < 0) {
            timer->cancel();
        }
    });

    recurring_task::start(io, boost::posix_time::seconds(1), boost::posix_time::seconds(2),
            [](recurring_task&) {
        std::cout << "recurring task!\n";
    });

    io.run();
    std::cout << "end\n";
    return 0;
}
