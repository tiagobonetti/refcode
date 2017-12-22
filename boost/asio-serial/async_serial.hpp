#pragma once

#include <boost/asio.hpp>
#include <boost/shared_array.hpp>
#include <boost/utility.hpp>

#include <functional>
#include <vector>

class async_serial : private boost::noncopyable {
   public:
    using baud_rate = boost::asio::serial_port_base::baud_rate;
    using parity = boost::asio::serial_port_base::parity;
    using character_size = boost::asio::serial_port_base::character_size;
    using flow_control = boost::asio::serial_port_base::flow_control;
    using stop_bits = boost::asio::serial_port_base::stop_bits;

    async_serial(boost::asio::io_service& io);

    async_serial(boost::asio::io_service& io,
                 const std::string& devname,
                 baud_rate baud_rate_,
                 parity parity_,
                 character_size character_size_,
                 flow_control flow_control_,
                 stop_bits stop_bits_);

    ~async_serial();

    void open(const std::string& devname,
              baud_rate baud_rate_,
              parity parity_,
              character_size character_size_,
              flow_control flow_control_,
              stop_bits stop_bits_);

    void close();

    void write(const char* data, size_t size);
    void write(const std::vector<char>& data);
    void write_string(const std::string& s);

    bool is_open() const { return open_; }
    bool is_error() const { return error_; };

    void set_read_callback(std::function<void(const char*, std::size_t)> callback_) { callback = callback_; }
    void clear_read_callback() { callback = nullptr; }

    boost::asio::io_service& io;

   private:
    void do_read();
    void do_read_end(const boost::system::error_code& error, size_t bytes_transferred);

    void do_write();
    void do_write_end(const boost::system::error_code& error);

    void do_close();

    void set_open(bool o) { open_ = o; }
    void set_error(bool e) { error_ = e; }

    boost::asio::serial_port port;

    bool open_;
    bool error_;
    std::vector<char> writer_queue;

    boost::shared_array<char> write_buffer;
    std::size_t write_buffer_size;

    char read_buffer[512];

    std::function<void(const char*, std::size_t)> callback;
};
