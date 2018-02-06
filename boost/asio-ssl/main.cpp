#include "client_connection.hpp"
#include "server.hpp"

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <ctime>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

std::vector<std::thread> create_threads(
    boost::asio::io_service &ioService, std::size_t number) {
    std::vector<std::thread> threads;
    std::generate_n(std::back_inserter(threads), number, [&] { return std::thread{[&] { ioService.run(); }}; });
    return threads;
}

std::vector<std::shared_ptr<client_connection>> create_clients(
    boost::asio::io_service &ioService, std::size_t message_size, std::size_t number) {
    boost::asio::ip::tcp::resolver resolver{ioService};
    auto iterator = resolver.resolve({"127.0.0.1", "5555"});

    std::vector<std::shared_ptr<client_connection>> clients;
    std::generate_n(std::back_inserter(clients), number, [&] {
        return std::make_shared<client_connection>(ioService, iterator, message_size);
    });

    return clients;
}

std::chrono::milliseconds measure_transfer_time(std::vector<std::shared_ptr<client_connection>> &clients, std::size_t messages) {
    auto startTime = std::chrono::steady_clock::now();

    for (auto &client : clients) {
        client->async_send(messages);
    }

    while (server_connection::running_connections() != 0)
        std::this_thread::sleep_for(std::chrono::milliseconds{10});

    auto stopTime = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(
        stopTime - startTime);
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        std::cout << "Usage: " << argv[0] << " threads connections messages message_size\n";
        return 1;
    }

    std::size_t threads_no = std::atoll(argv[1]);
    std::size_t connections = std::atoll(argv[2]);
    std::size_t messages = std::atoll(argv[3]);
    std::size_t message_size = std::atoll(argv[4]);

    boost::asio::io_service ioService;
    boost::asio::io_service::work idleWork{ioService};

    auto threads = create_threads(ioService, threads_no);
    server s{ioService, connections, messages, message_size};
    auto clients = create_clients(ioService, message_size, connections);

    auto dt = measure_transfer_time(clients, messages);
    auto sec = static_cast<double>(dt.count()) / 1000;
    auto mB = static_cast<double>(connections * messages * message_size) / 1024 / 1024;

    std::cout << mB << " mB sent and received in " << sec << " sec. (" << (mB / sec) << " MB/s)\n";

    ioService.stop();
    for (auto &thread : threads) {
        thread.join();
    }

    return 0;
}
