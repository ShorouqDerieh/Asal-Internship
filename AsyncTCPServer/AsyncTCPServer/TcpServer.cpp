#include "TcpServer.h"
#include "ClientSession.h"
#include <iostream>
#include <memory>
#include <utility>
using tcp = boost::asio::ip::tcp;

TcpServer::TcpServer(
    boost::asio::io_context& ioContext,
    unsigned short port,
    std::shared_ptr<ClientManager> manager
)
    : ioContext(ioContext),
    acceptor(ioContext, tcp::endpoint(tcp::v4(), port)),
    manager_(std::move(manager))
{
}

void TcpServer::start() {
    std::cout << "Server started\n";
    doAccept();
}

void TcpServer::doAccept() {
    std::cout << "Waiting for client...\n";
    acceptor.async_accept(
        [this](const boost::system::error_code& ec,
            tcp::socket socket) {

                if (!ec) {
                    auto session = std::make_shared<ClientSession>(
                        std::move(socket),
                        manager_
                    );

                    session->start();
                }

                doAccept();
        }
    );
}
