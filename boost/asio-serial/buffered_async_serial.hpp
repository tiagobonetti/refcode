#pragma once
#include "async_serial.hpp"

class buffered_async_serial : public async_serial {
   public:
    buffered_async_serial();
    buffered_async_serial(const std::string& devname,
                        unsigned int baud_rate,
                        parity opt_parity = parity(parity::none),
                        character_size opt_csize = character_size(8),
                        flow_control opt_flow = flow_control(flow_control::none),
                        stop_bits opt_stop = stop_bits(stop_bits::one));

    std::size_t read(char* data, std::size_t size);
    std::vector<char> read();

    std::string read_string();
    std::string read_string_until(const std::string delim = "\n");

    virtual ~buffered_async_serial();

   private:
    void read_callback(const char* data, std::size_t len);

    static std::vector<char>::iterator find_string_in_vector(std::vector<char>& v, const std::string& s);

    std::vector<char> readQueue;
    boost::mutex readQueueMutex;
};
