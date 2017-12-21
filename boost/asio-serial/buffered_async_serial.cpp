#include "buffered_async_serial.hpp"

#include <boost/bind.hpp>

#include <algorithm>
#include <iostream>
#include <string>

// buffered_async_serial

buffered_async_serial::buffered_async_serial() : async_serial() {
    set_read_callback(boost::bind(&buffered_async_serial::read_callback, this, _1, _2));
}

buffered_async_serial::buffered_async_serial(const std::string& devname,
                                         unsigned int baud_rate,
                                         parity opt_parity,
                                         character_size opt_csize,
                                         flow_control opt_flow,
                                         stop_bits opt_stop)
    : async_serial(devname, baud_rate, opt_parity, opt_csize, opt_flow, opt_stop) {
    set_read_callback(boost::bind(&buffered_async_serial::read_callback, this, _1, _2));
}

std::size_t buffered_async_serial::read(char* data, std::size_t size) {
    boost::lock_guard<boost::mutex> l(readQueueMutex);
    std::size_t result = std::min(size, readQueue.size());
    std::vector<char>::iterator it = readQueue.begin() + result;
    std::copy(readQueue.begin(), it, data);
    readQueue.erase(readQueue.begin(), it);
    return result;
}

std::vector<char> buffered_async_serial::read() {
    boost::lock_guard<boost::mutex> l(readQueueMutex);
    std::vector<char> result;
    result.swap(readQueue);
    return result;
}

std::string buffered_async_serial::read_string() {
    boost::lock_guard<boost::mutex> l(readQueueMutex);
    std::string result(readQueue.begin(), readQueue.end());
    readQueue.clear();
    return result;
}

std::string buffered_async_serial::read_string_until(const std::string delim) {
    boost::lock_guard<boost::mutex> l(readQueueMutex);
    std::vector<char>::iterator it = find_string_in_vector(readQueue, delim);
    if (it == readQueue.end()) return "";
    std::string result(readQueue.begin(), it);
    it += delim.size();  //Do remove the delimiter from the queue
    readQueue.erase(readQueue.begin(), it);
    return result;
}

void buffered_async_serial::read_callback(const char* data, std::size_t len) {
    boost::lock_guard<boost::mutex> l(readQueueMutex);
    readQueue.insert(readQueue.end(), data, data + len);
}

std::vector<char>::iterator buffered_async_serial::find_string_in_vector(std::vector<char>& v, const std::string& s) {
    if (s.size() == 0) return v.end();

    std::vector<char>::iterator it = v.begin();
    for (;;) {
        std::vector<char>::iterator result = find(it, v.end(), s[0]);
        if (result == v.end()) return v.end();  //If not found return

        for (std::size_t i = 0; i < s.size(); i++) {
            std::vector<char>::iterator temp = result + i;
            if (temp == v.end()) return v.end();
            if (s[i] != *temp) goto mismatch;
        }
        //Found
        return result;

    mismatch:
        it = result + 1;
    }
}

buffered_async_serial::~buffered_async_serial() {
    clear_read_callback();
}
