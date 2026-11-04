#pragma once
#include <boost/asio.hpp>
#include <memory>
class ClientManager;
class TcpServer {
private:
    boost::asio::io_context& ioContext;
    boost::asio::ip::tcp::acceptor acceptor;
    std::shared_ptr<ClientManager> manager_;

    void doAccept();
public:
    TcpServer(boost::asio::io_context& ioContext,unsigned short port,std::shared_ptr<ClientManager> manager);
    void start();
};
