#include "ClientSession.h"
#include <iostream>
#include <istream>
#include <utility>
#include <memory>
#include <string>
#include <boost/asio/write.hpp>
#include <thread>
#include "ClientManager.h"
#include <algorithm>
using tcp = boost::asio::ip::tcp;

ClientSession::ClientSession(
    tcp::socket socket,
    std::weak_ptr<ClientManager> manager
)
    : socket_(std::move(socket)),
    strand_(
        boost::asio::make_strand(socket_.get_executor())
    ),
    manager_(std::move(manager))
{
}

void ClientSession::start() {
    auto self = shared_from_this();

    boost::asio::post(
        strand_,
        [self]() {
            self->enqueue("Enter your username:\n");
            self->doRead();
        }
    );
}
void ClientSession::doRead() {
    if (stopped_) {
        return;
    }

    auto self = shared_from_this();

    boost::asio::async_read_until(
        socket_,
        buffer_,
        '\n',

        boost::asio::bind_executor(
            strand_,
            [self](
                const boost::system::error_code& ec,
                std::size_t bytesTransferred
                ) {
                    if (ec) {
                        if (ec == boost::asio::error::eof) {
                            std::cout << "Client disconnected\n";
                        }
                        else if (ec != boost::asio::error::operation_aborted) {
                            std::cerr << "Read error: "
                                << ec.message() << '\n';
                        }

                        self->stop();
                        return;
                    }

                    std::istream input(&self->buffer_);

                    std::string message;
                    std::getline(input, message);

                    if (!message.empty() &&
                        message.back() == '\r') {
                        message.pop_back();
                    }

                    if (!self->registered_) {
                        self->handleRegistration(message);
                    }
                    else {
                        std::cout << self->username_
                            << ": " << message << '\n';

                        self->enqueue(
                            "Server received: " + message + "\n"
                        );
                    }

                    self->doRead();
            }
        )
    );
}
void ClientSession::send(std::string message) {
    auto self = shared_from_this();

    boost::asio::post(
        strand_,
        [self, message = std::move(message)]() mutable {
            self->enqueue(std::move(message));
        }
    );
}
void ClientSession::doWrite() {
    if (stopped_ || writeQueue_.empty()) {
        return;
    }

    auto self = shared_from_this();

    boost::asio::async_write(
        socket_,
        boost::asio::buffer(writeQueue_.front()),

        boost::asio::bind_executor(
            strand_,
            [self](
                const boost::system::error_code& ec,
                std::size_t bytesTransferred
                ) {
                    if (ec) {
                        self->stop();
                        return;
                    }

                    self->writeQueue_.pop_front();

                    if (!self->writeQueue_.empty()) {
                        self->doWrite();
                    }
            }
        )
    );
}
void ClientSession::enqueue(std::string message) {
    if (stopped_) {
        return;
    }

    bool writeInProgress = !writeQueue_.empty();

    writeQueue_.push_back(std::move(message));

    if (!writeInProgress) {
        doWrite();
    }
}
void ClientSession::stop() {
    if (stopped_) {
        return;
    }

    stopped_ = true;

    if (registered_) {
        if (auto manager = manager_.lock()) {
            manager->removeClient(username_, this);
        }

        registered_ = false;
    }

    boost::system::error_code ec;
    socket_.close(ec);
}
void ClientSession::handleRegistration(
    const std::string& username
) {
    bool valid = !username.empty() &&
        username.size() <= 20;

    for (char ch : username) {
        bool allowed =
            (ch >= 'a' && ch <= 'z') ||
            (ch >= 'A' && ch <= 'Z') ||
            (ch >= '0' && ch <= '9') ||
            ch == '_';

        if (!allowed) {
            valid = false;
            break;
        }
    }

    if (!valid) {
        enqueue("Invalid username. Use 1-20 letters, numbers, or underscores.\n");
        return;
    }

    auto manager = manager_.lock();

    if (!manager) {
        stop();
        return;
    }

    if (!manager->addClient(username, shared_from_this())) {
        enqueue("Username already taken. Try another name.\n");
        return;
    }

    username_ = username;
    registered_ = true;

    enqueue("Username registered successfully.\n");
}
