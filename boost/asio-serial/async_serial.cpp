#include "async_serial.hpp"

#include "log.hpp"

#include <algorithm>
#include <iostream>
#include <string>

// async_serial

async_serial::async_serial(boost::asio::io_service& io_) : io(io_), port(io_), open_(false), error_(false) {}

async_serial::async_serial(boost::asio::io_service& io_,
                           const std::string& devname,
                           baud_rate baud_rate_,
                           parity parity_,
                           character_size character_size_,
                           flow_control flow_control_,
                           stop_bits stop_bits_)
    : async_serial(io_) {
    open(devname, baud_rate_, parity_, character_size_, flow_control_, stop_bits_);
}

void async_serial::open(const std::string& devname,
                        baud_rate baud_rate_,
                        parity parity_,
                        character_size character_size_,
                        flow_control flow_control_,
                        stop_bits stop_bits_) {
    if (is_open()) {
        close();
    }

    //If an exception is thrown, error remains true
    set_error(true);

    port.open(devname);
    port.set_option(baud_rate_);
    port.set_option(parity_);
    port.set_option(character_size_);
    port.set_option(flow_control_);
    port.set_option(stop_bits_);

    // Start reading
    do_read();

    //If we get here, no error
    set_error(false);
    set_open(true);
}

void async_serial::close() {
    if (!is_open()) {
        return;
    }

    set_open(false);
    do_close();

    if (is_error()) {
        throw(boost::system::system_error(boost::system::error_code(), "Error while closing the device"));
    }
}

void async_serial::write(const char* data, std::size_t size) {
    writer_queue.insert(writer_queue.end(), data, data + size);
    do_write();
}

void async_serial::write(const std::vector<char>& data) {
    writer_queue.insert(writer_queue.end(), data.begin(), data.end());
    do_write();
}

void async_serial::write_string(const std::string& s) {
    writer_queue.insert(writer_queue.end(), s.begin(), s.end());
    do_write();
}

async_serial::~async_serial() {
    if (is_open()) {
        try {
            close();
        } catch (...) {
            //Don't throw from a destructor
        }
    }
}

void async_serial::do_read() {
    port.async_read_some(boost::asio::buffer(read_buffer, sizeof(read_buffer)),
                         [this](const boost::system::error_code& error, std::size_t bytes_transferred) {
                             do_read_end(error, bytes_transferred);
                         });
}

void async_serial::do_read_end(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (error == boost::system::errc::operation_canceled) {
        return;
    }

    boost::asio::detail::throw_error(error, "async_serial::do_read_end");

    if (callback) {
        callback(read_buffer, bytes_transferred);
    }
    do_read();
}

void async_serial::do_write() {
    //If a write operation is already in progress, do nothing
    if (write_buffer == 0) {
        write_buffer_size = writer_queue.size();
        write_buffer.reset(new char[writer_queue.size()]);
        std::copy(writer_queue.begin(), writer_queue.end(), write_buffer.get());
        writer_queue.clear();

        boost::asio::async_write(port,
                                 boost::asio::buffer(write_buffer.get(), write_buffer_size),
                                 [this](const boost::system::error_code& error, std::size_t) {
                                     do_write_end(error);
                                 });
    }
}

void async_serial::do_write_end(const boost::system::error_code& error) {
    if (error) {
        set_error(true);
        do_close();
    }

    if (writer_queue.empty()) {
        write_buffer.reset();
        write_buffer_size = 0;
        return;
    }

    write_buffer_size = writer_queue.size();
    write_buffer.reset(new char[writer_queue.size()]);
    std::copy(writer_queue.begin(), writer_queue.end(), write_buffer.get());
    writer_queue.clear();

    boost::asio::async_write(port,
                             boost::asio::buffer(write_buffer.get(), write_buffer_size),
                             [this](const boost::system::error_code& error, std::size_t) {
                                 do_write_end(error);
                             });
}

void async_serial::do_close() {
    boost::system::error_code ec;

    port.cancel(ec);
    if (ec) {
        set_error(true);
    };

    port.close(ec);
    if (ec) {
        set_error(true);
    };
}
