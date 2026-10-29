#include <algorithm>
#include <utility>

#include "BookSystemManager.h"
#include "json.hpp"

using json = nlohmann::json;

BookSystemManager::BookSystemManager(std::vector<Book> initialBooks)
    : books(std::move(initialBooks)), nextBookId(1)
{
    for (const auto& book : books)
    {
        nextBookId = std::max(nextBookId, book.id + 1);
    }
}

void BookSystemManager::updateBook(const httplib::Request& req,
    httplib::Response& res)
{
    json body = json::parse(req.body);
    int id = std::stoi(req.path_params.at("id"));

    for (auto& book : books)
    {
        if (book.id == id)
        {
            book.name = body.at("name").get<std::string>();
            book.author = body.at("author").get<std::string>();

            json response = {
                {"id", book.id},
                {"name", book.name},
                {"author", book.author}
            };

            res.status = 200;
            res.set_content(response.dump(), "application/json");
            return;
        }
    }

    res.status = 404;
    res.set_content(
        R"({"error":"Book not found"})",
        "application/json"
    );
}

void BookSystemManager::getBooks(const httplib::Request& req, httplib::Response& res)
{

    json result = json::array();
    for (const auto& book : books)
    {
        result.push_back({
                {"id", book.id},
                {"name", book.name},
                {"author", book.author}
            });
    }
    res.status = 200;
    res.set_content(
        result.dump(),
        "application/json"
    );
}
void BookSystemManager::addBook(const httplib::Request& req,
    httplib::Response& res
    )
{
    json body = json::parse(req.body);
    int newId = nextBookId++;
    books.push_back({ newId,body.at("name").get<std::string>(),body.at("author").get<std::string>()
        });
    res.status = 201;
    json response = {
            {"id", newId},
            {"name", body.at("name").get<std::string>()},
            {"author", body.at("author").get<std::string>()}
    };
    res.set_content(
        response.dump(),
        "application/json"
    );
}
void BookSystemManager::deleteBook(const httplib::Request& req, httplib::Response& res)
{
    int id = std::stoi(req.path_params.at("id"));
    for (auto it = books.begin(); it != books.end(); ++it)
    {
        if (it->id == id)
        {
            books.erase(it);
            res.status = 204;
            return;
        }
    }
    res.status = 404;
    res.set_content(
        R"({"error":"Book not found"})",
        "application/json"
    );
}
