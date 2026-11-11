#include "ClientManager.h"
#include "ClientSession.h"
#include <utility>

bool ClientManager::addClient(
    const std::string& username,
    std::shared_ptr<ClientSession> session
) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto [it, inserted] = clients_.emplace(
        username,
        std::move(session)
    );

    return inserted;
}
void ClientManager::removeClient(
    const std::string& username,
    const ClientSession* expectedSession
) {
    std::lock_guard<std::mutex> lock(mutex_);

    auto it = clients_.find(username);

    if (it != clients_.end() &&
        it->second.get() == expectedSession) {
        clients_.erase(it);
    }
}
std::vector<std::string>
ClientManager::getUsers() const {

    std::lock_guard<std::mutex> lock(mutex_);

    std::vector<std::string> users;

    for (const auto& [username, session] : clients_) {
        users.push_back(username);
    }

    return users;
}
void ClientManager::broadcast(
    const std::string& sender,
    const std::string& message
) {
    std::vector<std::shared_ptr<ClientSession>> recipients;

    {
        std::lock_guard<std::mutex> lock(mutex_);

        for (const auto& [username, session] : clients_) {
            if (username != sender) {
                recipients.push_back(session);
            }
        }
    }

    std::string formattedMessage =
        sender + ": " + message + "\n";

    for (const auto& session : recipients) {
        session->send(formattedMessage);
    }
}
bool ClientManager::sendPrivate(
    const std::string& sender,
    const std::string& target,
    const std::string& message
) {
    std::shared_ptr<ClientSession> recipient;

    {
        std::lock_guard<std::mutex> lock(mutex_);

        auto it = clients_.find(target);

        if (it == clients_.end()) {
            return false;
        }

        recipient = it->second;
    }

    recipient->send(
        "[Private] " + sender + ": " + message + "\n"
    );

    return true;
}
