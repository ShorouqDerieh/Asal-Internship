#pragma once
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

class ClientSession;

class ClientManager {
private:
    std::unordered_map<std::string,std::shared_ptr<ClientSession>> m_clients;

    mutable std::mutex mutex_;

public:
    bool addClient(std::string_view username,std::shared_ptr<ClientSession> session);

    void removeClient(std::string_view username,const ClientSession* expectedSession);

    std::vector<std::string> getUsers() const;

    void broadcast(std::string_view sender,std::string_view message);

    bool sendPrivate(std::string_view sender,
    std::string_view target,
    std::string_view message);
};
