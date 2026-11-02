#include "ClientSession.h"
#include <iostream>
#include <istream>
#include <utility>

using tcp = boost::asio::ip::tcp;

ClientSession::ClientSession(tcp::socket socket)
    : socket_(std::move(socket))
{
}

void ClientSession::start() {
    doRead();
}

void ClientSession::doRead() {
    auto self = shared_from_this();

    boost::asio::async_read_until(
        socket_,
        buffer_,
        '\n',
        [this, self](
            const boost::system::error_code& ec,
            std::size_t bytesTransferred
            ) {
                if (ec == boost::asio::error::eof) {
                    std::cout << "Client disconnected\n";
                    return;
                }

                if (ec) {
                    std::cerr << "Read error: "
                        << ec.message() << '\n';
                    return;
                }

                std::istream input(&buffer_);
                std::string message;
                std::getline(input, message);

                if (!message.empty() && message.back() == '\r') {
                    message.pop_back();
                }

                std::cout << "Received: " << message << '\n';

                doRead();
        }
    );
}
