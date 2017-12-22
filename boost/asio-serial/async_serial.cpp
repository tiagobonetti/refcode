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
    std::size_t result = std::min(size, read_queue.size());

    std::vector<char>::iterator it = read_queue.begin() + result;
    std::copy(read_queue.begin(), it, data);
    read_queue.erase(read_queue.begin(), it);

    return result;
}

std::vector<char> async_serial::read() {
    std::vector<char> result;
    result.swap(read_queue);
    return result;
}

std::string async_serial::read_string() {
    std::string result(read_queue.begin(), read_queue.end());
    read_queue.clear();
    return result;
}

std::string async_serial::read_string_until(const std::string delim) {
    std::vector<char>::iterator it = find_string_in_vector(read_queue, delim);
    if (it == read_queue.end()) {
        return "";
    }

    std::string result(read_queue.begin(), it);
    it += delim.size();  //Do remove the delimiter from the queue
    read_queue.erase(read_queue.begin(), it);

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
                             do_read_end(error, bytes_transferred);
                         });
}

void async_serial::do_read_end(const boost::system::error_code& error, std::size_t bytes_transferred) {
    if (error == boost::system::errc::operation_canceled) {
        return;
    }

    boost::asio::detail::throw_error(error, "async_serial::do_read_end");

    read_callback(read_buffer, bytes_transferred);
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

void async_serial::read_callback(const char* data, std::size_t len) {
    read_queue.insert(read_queue.end(), data, data + len);
}

std::vector<char>::iterator async_serial::find_string_in_vector(std::vector<char>& v, const std::string& s) {
    if (s.size() == 0) {
        return v.end();
    }

    std::vector<char>::iterator it = v.begin();
    for (;;) {
        std::vector<char>::iterator result = find(it, v.end(), s[0]);
        if (result == v.end()) {
            return v.end();  //If not found return
        }

        for (std::size_t i = 0; i < s.size(); i++) {
            std::vector<char>::iterator temp = result + i;

            if (temp == v.end()) {
                return v.end();
            }

            if (s[i] != *temp) {
                goto mismatch;
            }
        }
        //Found
        return result;

    mismatch:
        it = result + 1;
    }
}
