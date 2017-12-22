#pragma once
#include "async_serial.hpp"

class buffered_async_serial : public async_serial {
   public:
    buffered_async_serial(boost::asio::io_service& io);
    buffered_async_serial(boost::asio::io_service& io,
                          const std::string& devname,
                          baud_rate baud_rate_ = buffered_async_serial::baud_rate(57600),
                          parity parity_ = parity(parity::none),
                          character_size character_size_ = character_size(8),
                          flow_control flow_control_ = flow_control(flow_control::none),
                          stop_bits stop_bits_ = stop_bits(stop_bits::one));

    std::size_t read(char* data, std::size_t size);
    std::vector<char> read();

    std::string read_string();
    std::string read_string_until(const std::string delim = "\n");

   private:
    void read_callback(const char* data, std::size_t len);

    static std::vector<char>::iterator find_string_in_vector(std::vector<char>& v, const std::string& s);

    std::vector<char> read_queue;
};
