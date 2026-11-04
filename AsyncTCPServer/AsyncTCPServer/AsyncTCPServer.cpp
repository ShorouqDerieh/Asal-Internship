
#include <boost/asio.hpp>

#include <charconv>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <memory>

#include "TcpServer.h"
#include "ClientManager.h"

bool parseUnsigned(
    const std::string& text,
    unsigned int& value
) {
    auto result = std::from_chars(
        text.data(),
        text.data() + text.size(),
        value
    );

    return result.ec == std::errc{} &&
        result.ptr == text.data() + text.size();
}

int main(int argc, char* argv[]) {

    if (argc != 3) {
        std::cerr
            << "Usage: AsyncTCPServer <port> <threads>\n";
        return 1;
    }

    unsigned int portNumber = 0;
    unsigned int threadCount = 0;

    if (!parseUnsigned(argv[1], portNumber) ||
        portNumber == 0 || portNumber > 65535) {

        std::cerr << "Invalid port number\n";
        return 1;
    }

    if (!parseUnsigned(argv[2], threadCount) ||
        threadCount == 0 || threadCount > 64) {

        std::cerr << "Invalid thread count (1-64)\n";
        return 1;
    }

    try {
        boost::asio::io_context ioContext;

        auto manager = std::make_shared<ClientManager>();

        TcpServer server(ioContext, static_cast<unsigned short>(portNumber), manager);

        server.start();

        std::vector<std::thread> workers;
        std::mutex coutMutex;

        for (unsigned int i = 0; i < threadCount; ++i) {
            workers.emplace_back([&ioContext, &coutMutex]() {
                {
                    std::lock_guard<std::mutex> lock(coutMutex);

                    std::cout << "Worker started: "
                        << std::this_thread::get_id()
                        << '\n';
                }

                ioContext.run();
                });
        }

        for (auto& worker : workers) {
            worker.join();
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Server error: "
            << e.what() << '\n';
        return 1;
    }

    return 0;
}
