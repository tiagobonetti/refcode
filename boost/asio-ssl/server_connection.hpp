#pragma once

#include "server_connection.hpp"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <atomic>
#include <memory>
#include <vector>

class server_connection {
   public:
    server_connection(boost::asio::io_service &ioService, boost::asio::ssl::context &context, std::size_t messageSize)
        : m_socket{ioService, context}, m_buffer(messageSize) {
        ++s_runningConnections;
    }

    ~server_connection() { --s_runningConnections; }

    boost::asio::ssl::stream<boost::asio::ip::tcp::socket>::lowest_layer_type &socket() {
        return m_socket.lowest_layer();
    }

    void start(std::shared_ptr<server_connection> self, std::size_t messages) {
        m_socket.async_handshake(boost::asio::ssl::stream_base::server,
                                 [=](const boost::system::error_code &) { async_read(self, messages); });
    }

    static std::size_t running_connections() { return s_runningConnections; }

   private:
    void async_read(std::shared_ptr<server_connection> self, std::size_t messages) {
        boost::asio::async_read(m_socket, boost::asio::buffer(m_buffer),
                                [=](const boost::system::error_code &, std::size_t) {
                                    if (messages > 1)
                                        async_read(self, messages - 1);
                                });
    }

    static std::atomic<std::size_t> s_runningConnections;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> m_socket;
    std::vector<char> m_buffer;
};

std::atomic<std::size_t> server_connection::s_runningConnections{0};
