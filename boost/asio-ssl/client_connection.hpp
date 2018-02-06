#pragma once

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <vector>

class client_connection {
   public:
    client_connection(boost::asio::io_service &ioService, boost::asio::ip::tcp::resolver::iterator iterator, std::size_t messageSize)
        : m_socket{ioService, m_context}, m_buffer(messageSize) {
        boost::asio::connect(m_socket.lowest_layer(), iterator);
        m_socket.handshake(boost::asio::ssl::stream_base::client);
    }

    void async_send(std::size_t messages) {
        boost::asio::async_write(m_socket, boost::asio::buffer(m_buffer),
                                 [=](const boost::system::error_code &, std::size_t) {
                                     if (messages > 1)
                                         async_send(messages - 1);
                                 });
    }

   private:
    boost::asio::ssl::context m_context{boost::asio::ssl::context::tlsv12_client};
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> m_socket;
    std::vector<char> m_buffer;
};
