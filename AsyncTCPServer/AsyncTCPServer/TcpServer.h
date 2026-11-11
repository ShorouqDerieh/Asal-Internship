#pragma once

#include <boost/asio.hpp>

#include <memory>
#include <vector>

class ClientManager;
class ClientSession;

class TcpServer {
private:
    boost::asio::io_context& ioContext;

    boost::asio::strand<
        boost::asio::io_context::executor_type
    > acceptStrand_;

    boost::asio::ip::tcp::acceptor acceptor;

    std::shared_ptr<ClientManager> manager_;

    std::vector<std::weak_ptr<ClientSession>> sessions_;

    bool stopped_ = false;

    void doAccept();

public:
    TcpServer(
        boost::asio::io_context& ioContext,
        unsigned short port,
        std::shared_ptr<ClientManager> manager
    );

    void start();
    void stop();
};
