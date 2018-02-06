#pragma once

#include "server_connection.hpp"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

class server {
   public:
    server(boost::asio::io_service &ioService, std::size_t connections, std::size_t messages, std::size_t messageSize)
        : m_ioService{ioService}, m_messages{messages}, m_messageSize{messageSize} {
        m_context.use_certificate_chain_file("server.pem");
        m_context.use_private_key_file("server.key", boost::asio::ssl::context::pem);
        async_accept(connections);
    }

   private:
    void async_accept(std::size_t connections) {
        auto conn = std::make_shared<server_connection>(m_ioService, m_context, m_messageSize);

        m_acceptor.async_accept(conn->socket(), [=](const boost::system::error_code &) {
            conn->start(conn, m_messages);
            if (connections > 1)
                async_accept(connections - 1);
        });
    }

    boost::asio::io_service &m_ioService;
    std::size_t m_messages;
    std::size_t m_messageSize;

    boost::asio::ssl::context m_context{boost::asio::ssl::context::tlsv12_server};
    boost::asio::ip::tcp::acceptor m_acceptor{m_ioService, {boost::asio::ip::tcp::v4(), 5555}};
};
