#include "buffered_async_serial.hpp"

#include "log.hpp"

#include <boost/bind.hpp>

#include <algorithm>
#include <iostream>
#include <string>

// buffered_async_serial

buffered_async_serial::buffered_async_serial(boost::asio::io_service& io) : async_serial(io) {
    set_read_callback([this](const char* c, std::size_t s) { read_callback(c, s); });
}

buffered_async_serial::buffered_async_serial(boost::asio::io_service& io,
                                             const std::string& devname,
                                             baud_rate baud_rate_,
                                             parity parity_,
                                             character_size character_size_,
                                             flow_control flow_control_,
                                             stop_bits stop_bits_)
    : async_serial(io, devname, baud_rate_, parity_, character_size_, flow_control_, stop_bits_) {
    set_read_callback([this](const char* c, std::size_t s) { read_callback(c, s); });
}

std::size_t buffered_async_serial::read(char* data, std::size_t size) {
    std::size_t result = std::min(size, read_queue.size());

    std::vector<char>::iterator it = read_queue.begin() + result;
    std::copy(read_queue.begin(), it, data);
    read_queue.erase(read_queue.begin(), it);

    return result;
}

std::vector<char> buffered_async_serial::read() {
    std::vector<char> result;
    result.swap(read_queue);
    return result;
}

std::string buffered_async_serial::read_string() {
    std::string result(read_queue.begin(), read_queue.end());
    read_queue.clear();
    return result;
}

std::string buffered_async_serial::read_string_until(const std::string delim) {
    std::vector<char>::iterator it = find_string_in_vector(read_queue, delim);
    if (it == read_queue.end()) {
        return "";
    }

    std::string result(read_queue.begin(), it);
    it += delim.size();  //Do remove the delimiter from the queue
    read_queue.erase(read_queue.begin(), it);

    return result;
}

void buffered_async_serial::read_callback(const char* data, std::size_t len) {
    read_queue.insert(read_queue.end(), data, data + len);
}

std::vector<char>::iterator buffered_async_serial::find_string_in_vector(std::vector<char>& v, const std::string& s) {
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
