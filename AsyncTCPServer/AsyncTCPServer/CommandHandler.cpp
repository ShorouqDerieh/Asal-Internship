#include "CommandHandler.h"

#include <sstream>

ParsedCommand CommandHandler::parse(
    const std::string& input
) {
    std::istringstream stream(input);

    std::string command;
    stream >> command;

    if (command == "/users" || command == "/quit") {
        std::string extra;

        if (stream >> extra) {
            return {};
        }

        return {
            command == "/users"
                ? CommandType::Users
                : CommandType::Quit,
            "",
            ""
        };
    }

    if (command == "/msg") {
        std::string target;
        std::string message;

        if (!(stream >> target)) {
            return {};
        }

        std::getline(stream >> std::ws, message);

        if (message.empty()) {
            return {};
        }

        return {
            CommandType::PrivateMessage,
            target,
            message
        };
    }

    return {};
}
