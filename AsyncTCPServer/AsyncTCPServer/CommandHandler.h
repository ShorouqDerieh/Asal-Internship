#pragma once

#include <string>

enum class CommandType {
    Users,
    Quit,
    PrivateMessage,
    Invalid
};

struct ParsedCommand {
    CommandType type = CommandType::Invalid;
    std::string target;
    std::string message;
};

class CommandHandler {
public:
    static ParsedCommand parse(const std::string& input);
};
