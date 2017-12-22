#include "buffered_async_serial.hpp"

#include "log.hpp"

#include <boost/asio.hpp>

#include <iostream>

int main(int argc, char* argv[]) {
    boost::asio::io_service io;
    try {
        boost::asio::deadline_timer timer(io);
        buffered_async_serial serial(io, "/dev/ttyUSB0");

        serial.write_string("ls\n");

        timer.expires_from_now(boost::posix_time::seconds(1));
        timer.async_wait([&](const boost::system::error_code& error) {
            std::cout << serial.read_string() << '\n';
            serial.close();
        });


        io.run();
    } catch (boost::system::system_error& e) {
        std::cout << "Error: " << e.what() << '\n';
        return 1;
    }
}
