#pragma once
#pragma once

#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

class ClientSession;

class ClientManager {
private:
    std::unordered_map<
        std::string,
        std::shared_ptr<ClientSession>
    > clients_;

    mutable std::mutex mutex_;

public:
    bool addClient(
        const std::string& username,
        std::shared_ptr<ClientSession> session
    );

    void removeClient(
        const std::string& username,
        const ClientSession* expectedSession
    );
    std::vector<std::string> getUsers() const;
};
