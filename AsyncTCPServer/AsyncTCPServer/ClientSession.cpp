#include <iostream>
#include <istream>
#include <utility>
#include <memory>
#include <string>
#include <algorithm>
#include <thread>
#include <boost/asio/write.hpp>
#include "ClientManager.h"
#include "CommandHandler.h"
#include "ClientSession.h"
using tcp = boost::asio::ip::tcp;

ClientSession::ClientSession(tcp::socket socket,std::weak_ptr<ClientManager> manager)
    : m_socket(std::move(socket)),m_strand(boost::asio::make_strand(m_socket.get_executor())), m_manager(std::move(manager))
{
}

void ClientSession::start()
{
    auto self = shared_from_this();

    boost::asio::post(
        m_strand,
        [self]() {
            self->enqueue("Enter your username:\n");
            self->doRead();
        }
    );
}
void ClientSession::doRead()
{
    if (stopped_)
    {
        return;
    }

    auto self = shared_from_this();

    boost::asio::async_read_until(
        m_socket,
        m_buffer,
        '\n',

        boost::asio::bind_executor(
            m_strand,
            [self](
                const boost::system::error_code& ec,
                std::size_t bytesTransferred
                ) {
                    if (ec)
                    {
                        if (ec == boost::asio::error::eof)
                        {
                            std::cout << "Client disconnected\n";
                        }
                        else if (ec != boost::asio::error::operation_aborted)
                        {
                            std::cerr << "Read error: "
                                << ec.message() << '\n';
                        }

                        self->stop();
                        return;
                    }

                    std::istream input(&self->m_buffer);

                    std::string message;
                    std::getline(input, message);
                    if (!message.empty() && message.back() == '\r')
                    {
                        message.pop_back();
                    }
                    if (!self->m_registered_) {
                        self->handleRegistration(message);
                    }
                    else if (!message.empty() && message.front() == '/')
                    {
                        self->handleCommand(message);
                    }
                    else
                    {
                        if (auto manager = self->m_manager.lock())
                        {
                            manager->broadcast(self->m_username, message);
                        }
                        else
                        {
                            self->stop();
                            return;
                        }
                    }


                    self->doRead();
            }
        )
    );
}

void ClientSession::send(std::string message)
{
    auto self = shared_from_this();
    boost::asio::post(m_strand,
        [self, message = std::move(message)]() mutable {
            self->enqueue(std::move(message));
        }
    );
}
void ClientSession::doWrite()
{
    if (stopped_ || m_writeQueue.empty())
    {
        return;
    }

    auto self = shared_from_this();
    boost::asio::async_write( m_socket,
        boost::asio::buffer(m_writeQueue.front()),
        boost::asio::bind_executor(
            m_strand,
            [self](
                const boost::system::error_code& ec,
                std::size_t bytesTransferred
                ) {
                    if (ec)
                    {
                        self->stop();
                        return;
                    }

                    self->m_writeQueue.pop_front();

                    if (!self->m_writeQueue.empty())
                    {
                        self->doWrite();
                    }
            }
        )
    );
}
void ClientSession::enqueue(std::string message)
{
    if (stopped_)
    {
        return;
    }

    bool writeInProgress = !m_writeQueue.empty();

    m_writeQueue.push_back(std::move(message));

    if (!writeInProgress)
    {
        doWrite();
    }
}

void ClientSession::stop()
{
    if (stopped_)
    {
        return;
    }

    stopped_ = true;

    if (m_registered_)
    {
        if (auto manager = m_manager.lock())
        {
            manager->removeClient(m_username, this);
        }

        m_registered_ = false;
    }

    boost::system::error_code ec;
    m_socket.close(ec);
}

void ClientSession::handleRegistration(const std::string& username)
{
    bool valid = !username.empty() &&username.size() <= 20;

    for (char ch : username)
    {
        bool allowed =
            (ch >= 'a' && ch <= 'z') ||
            (ch >= 'A' && ch <= 'Z') ||
            (ch >= '0' && ch <= '9') ||
            ch == '_';

        if (!allowed)
        {
            valid = false;
            break;
        }
    }

    if (!valid)
    {
        enqueue("Invalid username. Use 1-20 letters, numbers, or underscores.\n");
        return;
    }

    auto manager = m_manager.lock();

    if (!manager)
    {
        stop();
        return;
    }

    if (!manager->addClient(username, shared_from_this()))
    {
        enqueue("Username already taken. Try another name.\n");
        return;
    }

    m_username = username;
    m_registered_ = true;

    enqueue("Username registered successfully.\n");
}
void ClientSession::handleCommand(const std::string& input)
{
    auto command = CommandHandler::parse(input);
    auto manager = m_manager.lock();

    if (!manager)
    {
        stop();
        return;
    }

    switch (command.type) {

    case CommandType::Users:
    {
        auto users = manager->getUsers();

        std::string response = "Connected users:\n";

        for (const auto& user : users)
        {
            response += "- " + user + "\n";
        }

        enqueue(response);
        break;
    }

    case CommandType::Quit:
        stop();
        break;

    case CommandType::PrivateMessage:
        if (!manager->sendPrivate(
            m_username,
            command.target,
            command.message)) {

            enqueue("User not found.\n");
        }
        break;

    case CommandType::Invalid:
        enqueue("Invalid command.\n");
        break;
    }
}
void ClientSession::requestStop()
{
    auto self = shared_from_this();

    boost::asio::post(
        m_strand,
        [self]() {
            self->stop();
        }
    );
}
