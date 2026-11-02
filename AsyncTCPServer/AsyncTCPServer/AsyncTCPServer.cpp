#include <iostream>
#include <boost/asio.hpp>
#include "TcpServer.h"

int main() {
    try {
        boost::asio::io_context ioContext;

        int port = 9091;

        TcpServer server(ioContext, port);

        server.start();

        ioContext.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Server error: "
            << e.what() << '\n';
        return 1;
    }

    return 0;
}
