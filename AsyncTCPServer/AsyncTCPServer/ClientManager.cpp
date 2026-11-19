#include "ClientManager.h"
#include "ClientSession.h"
#include <utility>
#include <format>
#include <string_view>

bool ClientManager::addClient(std::string_view username,std::shared_ptr<ClientSession> session)
{
    std::lock_guard<std::mutex> lock(mutex_);
    auto [it, inserted] = m_clients.emplace(username,std::move(session));
    return inserted;
}
void ClientManager::removeClient(std::string_view username,const ClientSession* expectedSession)
{
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = m_clients.find(std::string(username));

    if (it != m_clients.end() &&it->second.get() == expectedSession)
    {
        m_clients.erase(it);
    }
}
std::vector<std::string>ClientManager::getUsers() const
{
    std::lock_guard<std::mutex> lock(mutex_);

    std::vector<std::string> users;

    for (const auto& [username, session] : m_clients)
    {
        users.push_back(username);
    }

    return users;
}
void ClientManager::broadcast(std::string_view sender,std::string_view message) {
    std::vector<std::shared_ptr<ClientSession>> recipients;
    {
        std::lock_guard<std::mutex> lock(mutex_);

        for (const auto& [username, session] : m_clients)
        {
            if (username != sender)
            {
                recipients.push_back(session);
            }
        }
    }

    std::string formattedMessage =std::format("{}: {}\n", sender, message);

    for (const auto& session : recipients) {
        session->send(formattedMessage);
    }
}
bool ClientManager::sendPrivate(std::string_view sender,std::string_view target,std::string_view message)
{
    std::shared_ptr<ClientSession> recipient;

    {
        std::lock_guard<std::mutex> lock(mutex_);

        auto it = m_clients.find(std::string(target));

        if (it == m_clients.end())
        {
            return false;
        }

        recipient = it->second;
    }

    recipient->send(std::format("[Private] {}: {}\n", sender, message));

    return true;
}
