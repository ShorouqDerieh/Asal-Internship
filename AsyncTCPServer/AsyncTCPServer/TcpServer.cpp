#include "TcpServer.h"
#include "ClientSession.h"

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>

using tcp = boost::asio::ip::tcp;

TcpServer::TcpServer(
    boost::asio::io_context& ioContext,
    unsigned short port,
    std::shared_ptr<ClientManager> manager
)
    : ioContext(ioContext),
    acceptStrand_(boost::asio::make_strand(ioContext)),
    acceptor(ioContext, tcp::endpoint(tcp::v4(), port)),
    manager_(std::move(manager))
{
}

void TcpServer::start() {
    boost::asio::post(
        acceptStrand_,
        [this]() {
            std::cout << "Server started\n";
            doAccept();
        }
    );
}

void TcpServer::doAccept() {
    if (stopped_) {
        return;
    }

    std::cout << "Waiting for client...\n";

    acceptor.async_accept(
        boost::asio::bind_executor(
            acceptStrand_,
            [this](
                const boost::system::error_code& ec,
                tcp::socket socket
                ) {
                    if (stopped_) {
                        return;
                    }

                    if (ec) {
                        if (ec != boost::asio::error::operation_aborted) {
                            std::cerr << "Accept error: "
                                << ec.message() << '\n';
                        }
                    }
                    else {
                        auto session =
                            std::make_shared<ClientSession>(
                                std::move(socket),
                                manager_
                            );

                        sessions_.push_back(session);

                        session->start();

                        sessions_.erase(
                            std::remove_if(
                                sessions_.begin(),
                                sessions_.end(),
                                [](const auto& weakSession) {
                                    return weakSession.expired();
                                }
                            ),
                            sessions_.end()
                        );
                    }

                    if (!stopped_ && acceptor.is_open()) {
                        doAccept();
                    }
            }
        )
    );
}

void TcpServer::stop() {
    boost::asio::post(
        acceptStrand_,
        [this]() {
            if (stopped_) {
                return;
            }

            stopped_ = true;

            std::cout << "Stopping server...\n";

            boost::system::error_code ec;
            acceptor.close(ec);

            if (ec) {
                std::cerr << "Acceptor close error: "
                    << ec.message() << '\n';
            }

            for (const auto& weakSession : sessions_) {
                if (auto session = weakSession.lock()) {
                    session->requestStop();
                }
            }

            sessions_.clear();

            std::cout << "Shutdown initiated.\n";
        }
    );
}
