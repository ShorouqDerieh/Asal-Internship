#pragma once
#include <boost/asio.hpp>

class TcpServer {
private:
    boost::asio::io_context& ioContext;
    boost::asio::ip::tcp::acceptor acceptor;

    void doAccept();
public:
    TcpServer(
        boost::asio::io_context& ioContext,
        int port
    );

    void start();
};
