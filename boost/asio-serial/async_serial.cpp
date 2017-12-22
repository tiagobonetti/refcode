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

async_serial::~async_serial() {
    if (is_open()) {
        try {
            close();
        } catch (...) {
            //Don't throw from a destructor
        }
    }
}

// Open/Close

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

std::size_t async_serial::read(char* data, std::size_t size) {
    std::size_t read_size = std::min(size, read_queue.size());

    auto begin = std::begin(read_queue);
    auto end = begin + read_size;

    std::copy(begin, end, data);
    read_queue.erase(begin, end);

    return read_size;
}

std::vector<char> async_serial::read() {
    std::vector<char> result;
    result.swap(read_queue);
    return result;
}

std::string async_serial::read_string() {
    std::string result(std::begin(read_queue), std::end(read_queue));
    read_queue.clear();
    return result;
}

std::string async_serial::read_string_until(const std::string delimiter) {
    auto begin = std::begin(read_queue);
    auto it = std::search(std::begin(read_queue), std::end(read_queue), std::begin(delimiter), std::end(delimiter));
    if (it == std::end(read_queue)) {
        return "";
    }

    std::string result(std::begin(read_queue), it);

    // Remove the delimiter from the queue
    it += delimiter.size();
    // Remove result from queue
    read_queue.erase(std::begin(read_queue), it);

    return result;
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

void async_serial::do_read() {
    port.async_read_some(boost::asio::buffer(read_buffer, sizeof(read_buffer)),
                         [this](const boost::system::error_code& error, std::size_t bytes_transferred) {
                             handle_read(error, bytes_transferred);
                         });
}

void async_serial::handle_read(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (error == boost::system::errc::operation_canceled) {
        return;
    }

    if (error) {
        set_error(true);
        throw boost::system::system_error(error, "async_serial::handle_read");
    }

    auto begin = std::begin(read_buffer);
    auto end = begin + bytes_transferred;
    read_queue.insert(std::end(read_queue), begin, end);
    do_read();
}

void async_serial::do_write() {
    // If queue is empty, do nothing
    if (writer_queue.empty()) {
        return;
    }
    // If a write operation is already in progress, do nothing
    if (!writer_buffer.empty()) {
        return;
    }

    std::swap(writer_queue, writer_buffer);
    boost::asio::async_write(port,
                             boost::asio::buffer(writer_buffer, writer_buffer.size()),
                             [this](const boost::system::error_code& error, std::size_t) {
                                 handle_write(error);
                             });
}

void async_serial::handle_write(const boost::system::error_code& error) {
    if (error == boost::system::errc::operation_canceled) {
        return;
    }

    if (error) {
        set_error(true);
        throw boost::system::system_error(error, "async_serial::handle_read");
    }

    // clean written buffer and try to keep writing
    writer_buffer.clear();
    do_write();
}
