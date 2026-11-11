#define BOOST_TEST_MODULE AsyncTCPServerTests

#include <boost/test/included/unit_test.hpp>

#include "CommandHandler.h"

BOOST_AUTO_TEST_CASE(UsersCommandTest)
{
    auto result = CommandHandler::parse("/users");

    BOOST_CHECK(result.type == CommandType::Users);
}
BOOST_AUTO_TEST_CASE(QuitCommandTest)
{
    auto result = CommandHandler::parse("/quit");

    BOOST_CHECK(result.type == CommandType::Quit);
}

BOOST_AUTO_TEST_CASE(PrivateMessageTest)
{
    auto result =
        CommandHandler::parse("/msg Bob Hello Bob!");

    BOOST_CHECK(
        result.type == CommandType::PrivateMessage
    );

    BOOST_CHECK(result.target == "Bob");
    BOOST_CHECK(result.message == "Hello Bob!");
}

BOOST_AUTO_TEST_CASE(InvalidCommandTest)
{
    auto result = CommandHandler::parse("/unknown");

    BOOST_CHECK(result.type == CommandType::Invalid);
}

BOOST_AUTO_TEST_CASE(MissingUsernameTest)
{
    auto result = CommandHandler::parse("/msg");

    BOOST_CHECK(result.type == CommandType::Invalid);
}

BOOST_AUTO_TEST_CASE(MissingMessageTest)
{
    auto result = CommandHandler::parse("/msg Bob");

    BOOST_CHECK(result.type == CommandType::Invalid);
}

BOOST_AUTO_TEST_CASE(UnexpectedArgumentsTest)
{
    auto result = CommandHandler::parse("/users extra");

    BOOST_CHECK(result.type == CommandType::Invalid);
}
