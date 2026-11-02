#pragma once

#include <boost/asio.hpp>
#include <boost/asio/streambuf.hpp>
#include <memory>

class ClientSession
    : public std::enable_shared_from_this<ClientSession> {

private:
    boost::asio::ip::tcp::socket socket_;
    boost::asio::streambuf buffer_;

    void doRead();

public:
    explicit ClientSession(
        boost::asio::ip::tcp::socket socket
    );

    void start();
};
