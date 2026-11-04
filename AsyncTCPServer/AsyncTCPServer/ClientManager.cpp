#include "ClientManager.h"

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
