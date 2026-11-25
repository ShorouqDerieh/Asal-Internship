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

    tcp::socket m_socket;

    boost::asio::strand<boost::asio::any_io_executor> m_strand;

    boost::asio::streambuf m_buffer;
    std::deque<std::string> m_writeQueue;
    std::string m_username;
    bool m_registered_ = false;

    void handleRegistration(const std::string& username);
    bool stopped_ = false;
    std::weak_ptr<ClientManager> m_manager;

    void doRead();
    void enqueue(std::string message);
    void doWrite();
    void stop();
    void handleCommand(const std::string& input);
public:
    ClientSession( tcp::socket socket,std::weak_ptr<ClientManager> manager);
    void start();
    void send(std::string message);
    void requestStop();
};
