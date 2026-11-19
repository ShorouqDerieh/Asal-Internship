#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>

#include <memory>
#include <string>
#include <thread>
#include <vector>

#include "ClientManager.h"
#include "ClientSession.h"

using tcp = boost::asio::ip::tcp;

BOOST_AUTO_TEST_CASE(AddClientTest)
{
    boost::asio::io_context ioContext;

    auto manager = std::make_shared<ClientManager>();

    auto session = std::make_shared<ClientSession>(
        tcp::socket(ioContext),
        manager
    );

    bool added = manager->addClient("Alice", session);

    BOOST_TEST(added == true);

    auto users = manager->getUsers();

    BOOST_TEST(users.size() == 1u);
    BOOST_TEST(users[0] == "Alice");
}

BOOST_AUTO_TEST_CASE(DuplicateUsernameTest)
{
    boost::asio::io_context ioContext;

    auto manager = std::make_shared<ClientManager>();

    auto firstSession = std::make_shared<ClientSession>(
        tcp::socket(ioContext),
        manager
    );

    auto secondSession = std::make_shared<ClientSession>(
        tcp::socket(ioContext),
        manager
    );

    bool firstAdded =
        manager->addClient("Alice", firstSession);

    bool secondAdded =
        manager->addClient("Alice", secondSession);

    BOOST_TEST(firstAdded == true);
    BOOST_TEST(secondAdded == false);

    BOOST_TEST(manager->getUsers().size() == 1u);
}

BOOST_AUTO_TEST_CASE(RemoveClientTest)
{
    boost::asio::io_context ioContext;

    auto manager = std::make_shared<ClientManager>();

    auto session = std::make_shared<ClientSession>(
        tcp::socket(ioContext),
        manager
    );

    manager->addClient("Alice", session);

    manager->removeClient("Alice", session.get());

    auto users = manager->getUsers();

    BOOST_TEST(users.empty());
}

BOOST_AUTO_TEST_CASE(RegisterAfterRemovalTest)
{
    boost::asio::io_context ioContext;

    auto manager = std::make_shared<ClientManager>();

    auto firstSession = std::make_shared<ClientSession>(
        tcp::socket(ioContext),
        manager
    );

    manager->addClient("Alice", firstSession);

    manager->removeClient("Alice", firstSession.get());

    auto secondSession = std::make_shared<ClientSession>(
        tcp::socket(ioContext),
        manager
    );

    bool added =
        manager->addClient("Alice", secondSession);

    BOOST_TEST(added == true);
    BOOST_TEST(manager->getUsers().size() == 1u);
}


BOOST_AUTO_TEST_CASE(ConcurrentClientOperationsTest)
{
    constexpr int threadCount = 8;
    constexpr int usersPerThread = 20;
    constexpr int totalUsers = threadCount * usersPerThread;

    boost::asio::io_context ioContext;
    auto manager = std::make_shared<ClientManager>();

    std::vector<std::shared_ptr<ClientSession>> sessions;
    sessions.reserve(totalUsers);

    // Prepare sessions before starting threads.
    for (int i = 0; i < totalUsers; ++i) {
        sessions.push_back(
            std::make_shared<ClientSession>(
                tcp::socket(ioContext),
                manager
            )
        );
    }

    std::vector<std::thread> workers;

    // Multiple threads register clients concurrently.
    for (int t = 0; t < threadCount; ++t) {
        workers.emplace_back([&, t]() {
            for (int i = 0; i < usersPerThread; ++i) {
                int index = t * usersPerThread + i;

                manager->addClient(
                    "User" + std::to_string(index),
                    sessions[index]
                );
            }
            });
    }

    for (auto& worker : workers) {
        worker.join();
    }

    // All unique usernames should be registered.
    BOOST_TEST(
        manager->getUsers().size() ==
        static_cast<std::size_t>(totalUsers)
    );

    workers.clear();

    // Multiple threads remove clients concurrently.
    for (int t = 0; t < threadCount; ++t) {
        workers.emplace_back([&, t]() {
            for (int i = 0; i < usersPerThread; ++i) {
                int index = t * usersPerThread + i;

                manager->removeClient(
                    "User" + std::to_string(index),
                    sessions[index].get()
                );
            }
            });
    }

    for (auto& worker : workers) {
        worker.join();
    }

    // No clients should remain.
    BOOST_TEST(manager->getUsers().empty());
}
