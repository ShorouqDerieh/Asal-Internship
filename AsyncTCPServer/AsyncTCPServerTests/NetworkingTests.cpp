
#include <boost/test/unit_test.hpp>
#include <boost/asio.hpp>

#include <chrono>
#include <memory>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "ClientManager.h"
#include "ClientSession.h"

using tcp = boost::asio::ip::tcp;
namespace asio = boost::asio;

// Send a complete line to a TCP client session.
void sendLine(tcp::socket& socket, const std::string& text)
{
    std::string data = text + "\n";
    asio::write(socket, asio::buffer(data));
}

// Read one line with a timeout.
std::string readLine(tcp::socket& socket)
{
    socket.non_blocking(true);

    std::string line;
    char ch;

    auto deadline = std::chrono::steady_clock::now()
        + std::chrono::seconds(3);

    while (std::chrono::steady_clock::now() < deadline)
    {
        boost::system::error_code ec;

        std::size_t count =
            socket.read_some(asio::buffer(&ch, 1), ec);

        if (!ec && count == 1)
        {
            if (ch == '\n')
            {
                socket.non_blocking(false);

                if (!line.empty() && line.back() == '\r')
                    line.pop_back();

                return line;
            }

            line += ch;
        }
        else if (ec == asio::error::would_block ||
            ec == asio::error::try_again)
        {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(2)
            );
        }
        else if (ec)
        {
            throw boost::system::system_error(ec);
        }
    }

    throw std::runtime_error("Timed out waiting for a line");
}

BOOST_AUTO_TEST_CASE(BroadcastingTest)
{
    asio::io_context ioContext;

    auto manager = std::make_shared<ClientManager>();

    tcp::acceptor acceptor(
        ioContext,
        tcp::endpoint(asio::ip::address_v4::loopback(), 0)
    );

    unsigned short port = acceptor.local_endpoint().port();

    // Create Alice's client connection.
    tcp::socket alice(ioContext);
    alice.connect(tcp::endpoint(
        asio::ip::address_v4::loopback(), port));

    tcp::socket aliceServerSocket = acceptor.accept();

    auto aliceSession = std::make_shared<ClientSession>(
        std::move(aliceServerSocket), manager
    );

    aliceSession->start();

    // Create Bob's client connection.
    tcp::socket bob(ioContext);
    bob.connect(tcp::endpoint(
        asio::ip::address_v4::loopback(), port));

    tcp::socket bobServerSocket = acceptor.accept();

    auto bobSession = std::make_shared<ClientSession>(
        std::move(bobServerSocket), manager
    );

    bobSession->start();

    // Run asynchronous handlers on a worker thread.
    std::thread worker([&ioContext]() {
        ioContext.run();
        });

    // Ensure worker cleanup even if an assertion fails.
    struct WorkerCleanup {
        asio::io_context& io;
        std::thread& thread;

        ~WorkerCleanup() {
            io.stop();
            if (thread.joinable())
                thread.join();
        }
    } cleanup{ ioContext, worker };

    BOOST_TEST(readLine(alice) == "Enter your username:");
    BOOST_TEST(readLine(bob) == "Enter your username:");

    sendLine(alice, "Alice");
    BOOST_TEST(
        readLine(alice) == "Username registered successfully."
    );

    sendLine(bob, "Bob");
    BOOST_TEST(
        readLine(bob) == "Username registered successfully."
    );

    // Alice broadcasts a message.
    sendLine(alice, "Hello Bob!");

    // Bob must receive Alice's message.
    BOOST_TEST(readLine(bob) == "Alice: Hello Bob!");
}

BOOST_AUTO_TEST_CASE(PrivateMessageNetworkingTest)
{
    asio::io_context ioContext;

    auto manager = std::make_shared<ClientManager>();

    tcp::acceptor acceptor(
        ioContext,
        tcp::endpoint(asio::ip::address_v4::loopback(), 0)
    );

    unsigned short port = acceptor.local_endpoint().port();

    // Create Alice's connection
    tcp::socket alice(ioContext);

    alice.connect(
        tcp::endpoint(
            asio::ip::address_v4::loopback(),
            port
        )
    );

    tcp::socket aliceServerSocket = acceptor.accept();

    auto aliceSession = std::make_shared<ClientSession>(
        std::move(aliceServerSocket),
        manager
    );

    aliceSession->start();

    // Create Bob's connection
    tcp::socket bob(ioContext);

    bob.connect(
        tcp::endpoint(
            asio::ip::address_v4::loopback(),
            port
        )
    );

    tcp::socket bobServerSocket = acceptor.accept();

    auto bobSession = std::make_shared<ClientSession>(
        std::move(bobServerSocket),
        manager
    );

    bobSession->start();

    // Run asynchronous operations
    std::thread worker([&ioContext]() {
        ioContext.run();
        });

    // Stop and join the worker when the test finishes
    struct WorkerCleanup {
        asio::io_context& io;
        std::thread& thread;

        ~WorkerCleanup() {
            io.stop();

            if (thread.joinable()) {
                thread.join();
            }
        }
    } cleanup{ ioContext, worker };

    // Read registration prompts
    BOOST_TEST(readLine(alice) == "Enter your username:");
    BOOST_TEST(readLine(bob) == "Enter your username:");

    // Register Alice
    sendLine(alice, "Alice");

    BOOST_TEST(
        readLine(alice) == "Username registered successfully."
    );

    // Register Bob
    sendLine(bob, "Bob");

    BOOST_TEST(
        readLine(bob) == "Username registered successfully."
    );

    // Send a private message from Alice to Bob
    sendLine(alice, "/msg Bob Hello Bob!");

    BOOST_TEST(
        readLine(bob) == "[Private] Alice: Hello Bob!"
    );

    // Send a private message to an unknown user
    sendLine(alice, "/msg Charlie Hello!");

    BOOST_TEST(
        readLine(alice) == "User not found."
    );
}

BOOST_AUTO_TEST_CASE(WriteQueueOrderingTest)
{
    asio::io_context ioContext;

    auto manager = std::make_shared<ClientManager>();

    tcp::acceptor acceptor(
        ioContext,
        tcp::endpoint(asio::ip::address_v4::loopback(), 0)
    );

    unsigned short port = acceptor.local_endpoint().port();

    // Alice connection
    tcp::socket alice(ioContext);

    alice.connect(tcp::endpoint(
        asio::ip::address_v4::loopback(), port
    ));

    tcp::socket aliceServerSocket = acceptor.accept();

    auto aliceSession = std::make_shared<ClientSession>(
        std::move(aliceServerSocket), manager
    );

    aliceSession->start();

    // Bob connection
    tcp::socket bob(ioContext);

    bob.connect(tcp::endpoint(
        asio::ip::address_v4::loopback(), port
    ));

    tcp::socket bobServerSocket = acceptor.accept();

    auto bobSession = std::make_shared<ClientSession>(
        std::move(bobServerSocket), manager
    );

    bobSession->start();

    // Run asynchronous handlers
    std::thread worker([&ioContext]() {
        ioContext.run();
        });

    struct WorkerCleanup {
        asio::io_context& io;
        std::thread& thread;

        ~WorkerCleanup() {
            io.stop();

            if (thread.joinable()) {
                thread.join();
            }
        }
    } cleanup{ ioContext, worker };

    // Read username prompts
    BOOST_TEST(readLine(alice) == "Enter your username:");
    BOOST_TEST(readLine(bob) == "Enter your username:");

    // Register Alice
    sendLine(alice, "Alice");

    BOOST_TEST(
        readLine(alice) == "Username registered successfully."
    );

    // Register Bob
    sendLine(bob, "Bob");

    BOOST_TEST(
        readLine(bob) == "Username registered successfully."
    );

    // Send messages quickly
    sendLine(alice, "Message 1");
    sendLine(alice, "Message 2");
    sendLine(alice, "Message 3");

    // Check received order
    BOOST_TEST(readLine(bob) == "Alice: Message 1");
    BOOST_TEST(readLine(bob) == "Alice: Message 2");
    BOOST_TEST(readLine(bob) == "Alice: Message 3");
}

BOOST_AUTO_TEST_CASE(UsernameRegistrationNetworkingTest)
{
    asio::io_context ioContext;

    auto manager = std::make_shared<ClientManager>();

    tcp::acceptor acceptor(
        ioContext,
        tcp::endpoint(asio::ip::address_v4::loopback(), 0)
    );

    unsigned short port =
        acceptor.local_endpoint().port();

    // Alice connection
    tcp::socket alice(ioContext);

    alice.connect(tcp::endpoint(
        asio::ip::address_v4::loopback(), port
    ));

    tcp::socket aliceServerSocket = acceptor.accept();

    auto aliceSession = std::make_shared<ClientSession>(
        std::move(aliceServerSocket), manager
    );

    aliceSession->start();

    // Second client connection
    tcp::socket bob(ioContext);

    bob.connect(tcp::endpoint(
        asio::ip::address_v4::loopback(), port
    ));

    tcp::socket bobServerSocket = acceptor.accept();

    auto bobSession = std::make_shared<ClientSession>(
        std::move(bobServerSocket), manager
    );

    bobSession->start();

    std::thread worker([&ioContext]() {
        ioContext.run();
        });

    struct WorkerCleanup {
        asio::io_context& io;
        std::thread& thread;

        ~WorkerCleanup() {
            io.stop();

            if (thread.joinable()) {
                thread.join();
            }
        }
    } cleanup{ ioContext, worker };

    // Both clients receive registration prompts.
    BOOST_TEST(readLine(alice) == "Enter your username:");
    BOOST_TEST(readLine(bob) == "Enter your username:");

    // Test 1: Invalid username.
    sendLine(alice, "Alice!");

    BOOST_TEST(
        readLine(alice) ==
        "Invalid username. Use 1-20 letters, numbers, or underscores."
    );

    // Test 2: Valid username.
    sendLine(alice, "Alice");

    BOOST_TEST(
        readLine(alice) ==
        "Username registered successfully."
    );

    // Test 3: Duplicate username.
    sendLine(bob, "Alice");

    BOOST_TEST(
        readLine(bob) ==
        "Username already taken. Try another name."
    );

    // Test 4: Retry with a unique username.
    sendLine(bob, "Bob");

    BOOST_TEST(
        readLine(bob) ==
        "Username registered successfully."
    );

    // Verify both users exist.
    auto users = manager->getUsers();

    BOOST_TEST(users.size() == 2u);
}
