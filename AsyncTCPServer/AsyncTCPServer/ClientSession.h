#pragma once

#include <boost/asio.hpp>
#include <boost/asio/streambuf.hpp>

#include <deque>
#include <memory>
#include <string>
class ClientManager;

class ClientSession
    : public std::enable_shared_from_this<ClientSession> {

private:
    using tcp = boost::asio::ip::tcp;

    tcp::socket socket_;

    boost::asio::strand<boost::asio::any_io_executor> strand_;

    boost::asio::streambuf buffer_;
    std::deque<std::string> writeQueue_;
    std::string username_;
    bool registered_ = false;

    void handleRegistration(const std::string& username);
    bool stopped_ = false;
    std::weak_ptr<ClientManager> manager_;

    void doRead();
    void enqueue(std::string message);
    void doWrite();
    void stop();

public:
    ClientSession( tcp::socket socket,std::weak_ptr<ClientManager> manager);
    void start();
    void send(std::string message);
};
