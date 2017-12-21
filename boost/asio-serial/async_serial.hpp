#pragma once

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/shared_array.hpp>
#include <boost/thread.hpp>
#include <boost/utility.hpp>

#include <vector>

class async_serial_impl;

class async_serial : private boost::noncopyable {
   public:
    using baud_rate = boost::asio::serial_port_base::baud_rate;
    using parity = boost::asio::serial_port_base::parity;
    using character_size = boost::asio::serial_port_base::character_size;
    using flow_control = boost::asio::serial_port_base::flow_control;
    using stop_bits = boost::asio::serial_port_base::stop_bits;

    async_serial();

    async_serial(const std::string& devname,
                 unsigned int baud_rate,
                 parity opt_parity = parity(parity::none),
                 character_size opt_csize = character_size(8),
                 flow_control opt_flow = flow_control(flow_control::none),
                 stop_bits opt_stop = stop_bits(stop_bits::one));

    void open(const std::string& devname,
              unsigned int baud_rate,
              parity opt_parity = parity(parity::none),
              character_size opt_csize = character_size(8),
              flow_control opt_flow = flow_control(flow_control::none),
              stop_bits opt_stop = stop_bits(stop_bits::one));

    bool is_open() const;
    bool error_status() const;
    void close();
    void write(const char* data, size_t size);
    void write(const std::vector<char>& data);
    void write_string(const std::string& s);

    virtual ~async_serial() = 0;

    static const int readBufferSize = 512;

   private:
    void do_read();
    void read_end(const boost::system::error_code& error, size_t bytes_transferred);

    void do_write();
    void write_end(const boost::system::error_code& error);
    void do_close();

    boost::shared_ptr<async_serial_impl> pimpl;

   protected:
    void set_error_status(bool e);
    void set_read_callback(const boost::function<void(const char*, size_t)>& callback);
    void clear_read_callback();
};

class callback_async_serial : public async_serial {
   public:
    callback_async_serial();

    callback_async_serial(const std::string& devname,
                          unsigned int baud_rate,
                          parity opt_parity = parity(parity::none),
                          character_size opt_csize = character_size(8),
                          flow_control opt_flow = flow_control(flow_control::none),
                          stop_bits opt_stop = stop_bits(stop_bits::one));

    void set_callback(const boost::function<void(const char*, size_t)>& callback);
    void clear_callback();

    virtual ~callback_async_serial();
};
