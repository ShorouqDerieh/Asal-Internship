#include "TcpServer.h"
#include "ClientSession.h"
#include <iostream>
#include <memory>
#include <utility>
using tcp = boost::asio::ip::tcp;

TcpServer::TcpServer(
    boost::asio::io_context& ioContext,
    int port
)
    : ioContext(ioContext),
    acceptor(
        ioContext,
        tcp::endpoint(tcp::v4(), port)
    )
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
                        std::move(socket)
                    );

                    session->start();
                }

                doAccept();
        }
    );
}
