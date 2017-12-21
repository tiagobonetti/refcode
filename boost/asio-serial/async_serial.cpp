#include "async_serial.hpp"

#include <boost/bind.hpp>

#include <algorithm>
#include <iostream>
#include <string>

// async_serial

class async_serial_impl : private boost::noncopyable {
   public:
    async_serial_impl() : io(), port(io), backgroundThread(), open(false), error(false) {}

    boost::asio::io_service io;
    boost::asio::serial_port port;
    boost::thread backgroundThread;
    bool open;
    bool error;
    mutable boost::mutex errorMutex;

    std::vector<char> writeQueue;
    boost::shared_array<char> writeBuffer;
    std::size_t writeBufferSize;
    boost::mutex writeQueueMutex;
    char readBuffer[async_serial::readBufferSize];

    boost::function<void(const char*, std::size_t)> callback;
};

async_serial::async_serial() : pimpl(new async_serial_impl) {
}

async_serial::async_serial(const std::string& devname, unsigned int baud_rate,
                           parity opt_parity,
                           character_size opt_csize,
                           flow_control opt_flow,
                           stop_bits opt_stop)
    : pimpl(new async_serial_impl) {
    open(devname, baud_rate, opt_parity, opt_csize, opt_flow, opt_stop);
}

void async_serial::open(const std::string& devname, unsigned int baud_rate,
                        parity opt_parity,
                        character_size opt_csize,
                        flow_control opt_flow,
                        stop_bits opt_stop) {
    if (is_open()) close();

    set_error_status(true);  //If an exception is thrown, error_ remains true
    pimpl->port.open(devname);
    pimpl->port.set_option(boost::asio::serial_port_base::baud_rate(baud_rate));
    pimpl->port.set_option(opt_parity);
    pimpl->port.set_option(opt_csize);
    pimpl->port.set_option(opt_flow);
    pimpl->port.set_option(opt_stop);

    //This gives some work to the io_service before it is started
    pimpl->io.post(boost::bind(&async_serial::do_read, this));

    boost::thread t(boost::bind(&boost::asio::io_service::run, &pimpl->io));
    pimpl->backgroundThread.swap(t);
    set_error_status(false);  //If we get here, no error
    pimpl->open = true;       //Port is now open
}

bool async_serial::is_open() const {
    return pimpl->open;
}

bool async_serial::error_status() const {
    boost::lock_guard<boost::mutex> l(pimpl->errorMutex);
    return pimpl->error;
}

void async_serial::close() {
    if (!is_open()) return;

    pimpl->open = false;
    pimpl->io.post(boost::bind(&async_serial::do_close, this));
    pimpl->backgroundThread.join();
    pimpl->io.reset();
    if (error_status()) {
        throw(boost::system::system_error(boost::system::error_code(),
                                          "Error while closing the device"));
    }
}

void async_serial::write(const char* data, std::size_t size) {
    {
        boost::lock_guard<boost::mutex> l(pimpl->writeQueueMutex);
        pimpl->writeQueue.insert(pimpl->writeQueue.end(), data, data + size);
    }
    pimpl->io.post(boost::bind(&async_serial::do_write, this));
}

void async_serial::write(const std::vector<char>& data) {
    {
        boost::lock_guard<boost::mutex> l(pimpl->writeQueueMutex);
        pimpl->writeQueue.insert(pimpl->writeQueue.end(), data.begin(),
                                 data.end());
    }
    pimpl->io.post(boost::bind(&async_serial::do_write, this));
}

void async_serial::write_string(const std::string& s) {
    {
        boost::lock_guard<boost::mutex> l(pimpl->writeQueueMutex);
        pimpl->writeQueue.insert(pimpl->writeQueue.end(), s.begin(), s.end());
    }
    pimpl->io.post(boost::bind(&async_serial::do_write, this));
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
    pimpl->port.async_read_some(boost::asio::buffer(pimpl->readBuffer, readBufferSize),
                                boost::bind(&async_serial::read_end,
                                            this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
}

void async_serial::read_end(const boost::system::error_code& error,
                            std::size_t bytes_transferred) {
    if (error) {
        //error can be true even because the serial port was closed.
        //In this case it is not a real error, so ignore
        if (is_open()) {
            do_close();
            set_error_status(true);
        }
    } else {
        if (pimpl->callback) pimpl->callback(pimpl->readBuffer,
                                             bytes_transferred);
        do_read();
    }
}

void async_serial::do_write() {
    //If a write operation is already in progress, do nothing
    if (pimpl->writeBuffer == 0) {
        boost::lock_guard<boost::mutex> l(pimpl->writeQueueMutex);
        pimpl->writeBufferSize = pimpl->writeQueue.size();
        pimpl->writeBuffer.reset(new char[pimpl->writeQueue.size()]);
        copy(pimpl->writeQueue.begin(), pimpl->writeQueue.end(),
             pimpl->writeBuffer.get());
        pimpl->writeQueue.clear();
        async_write(pimpl->port, boost::asio::buffer(pimpl->writeBuffer.get(), pimpl->writeBufferSize),
                    boost::bind(&async_serial::write_end, this, boost::asio::placeholders::error));
    }
}

void async_serial::write_end(const boost::system::error_code& error) {
    if (!error) {
        boost::lock_guard<boost::mutex> l(pimpl->writeQueueMutex);
        if (pimpl->writeQueue.empty()) {
            pimpl->writeBuffer.reset();
            pimpl->writeBufferSize = 0;

            return;
        }
        pimpl->writeBufferSize = pimpl->writeQueue.size();
        pimpl->writeBuffer.reset(new char[pimpl->writeQueue.size()]);
        copy(pimpl->writeQueue.begin(), pimpl->writeQueue.end(),
             pimpl->writeBuffer.get());
        pimpl->writeQueue.clear();
        async_write(pimpl->port, boost::asio::buffer(pimpl->writeBuffer.get(), pimpl->writeBufferSize),
                    boost::bind(&async_serial::write_end, this, boost::asio::placeholders::error));
    } else {
        set_error_status(true);
        do_close();
    }
}

void async_serial::do_close() {
    boost::system::error_code ec;
    pimpl->port.cancel(ec);
    if (ec) set_error_status(true);
    pimpl->port.close(ec);
    if (ec) set_error_status(true);
}

void async_serial::set_error_status(bool e) {
    boost::lock_guard<boost::mutex> l(pimpl->errorMutex);
    pimpl->error = e;
}

void async_serial::set_read_callback(const boost::function<void(const char*, std::size_t)>& callback) {
    pimpl->callback = callback;
}

void async_serial::clear_read_callback() {
    pimpl->callback.clear();
}

// Class callback_async_serial

callback_async_serial::callback_async_serial() : async_serial() {
}

callback_async_serial::callback_async_serial(const std::string& devname,
                                             unsigned int baud_rate,
                                             parity opt_parity,
                                             character_size opt_csize,
                                             flow_control opt_flow,
                                             stop_bits opt_stop)
    : async_serial(devname, baud_rate, opt_parity, opt_csize, opt_flow, opt_stop) {
}

void callback_async_serial::set_callback(const boost::function<void(const char*, std::size_t)>& callback) {
    set_read_callback(callback);
}

void callback_async_serial::clear_callback() {
    clear_read_callback();
}

callback_async_serial::~callback_async_serial() {
    clear_read_callback();
}
