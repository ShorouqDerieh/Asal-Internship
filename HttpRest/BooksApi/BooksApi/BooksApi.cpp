#include <iostream>
#include <vector>
#include <string>
#include "httplib.h"
#include "json.hpp"
using json = nlohmann::json;

struct Book {
    int id;
    std::string name;
    std::string author;
};

void updateBook(const httplib::Request& req,httplib::Response& res,std::vector<Book>& books);
void getBooks(const httplib::Request& req, httplib::Response& res, std::vector<Book>& books);
void addBook(const httplib::Request& req, httplib::Response& res, std::vector<Book>& books, int& nextBookId);
void deleteBook(const httplib::Request& req, httplib::Response& res, std::vector<Book>& books);

int main() {
    httplib::Server server;
    std::vector<Book> books = {
    {1, "Clean Code", "Robert C. Martin"},
    {2, "The Pragmatic Programmer", "Andrew Hunt"}
    };
    int nextBookId = 3;
    server.Get("/books",
        [&](const httplib::Request& req,
            httplib::Response& res) {
                getBooks(req, res, books);
        });
    server.Post("/books",
        [&](const httplib::Request& req,
            httplib::Response& res) {
                addBook(req, res,books, nextBookId);
        }
    );
    server.Put("/books/:id",
        [&](const httplib::Request& req,
            httplib::Response& res)
        {
            updateBook(req, res, books);
        }
    );
    server.Delete("/books/:id", [&](const httplib::Request& req,
        httplib::Response& res) {
            deleteBook(req, res, books);
        });

    std::cout << "Server running at http://localhost:9090\n";

    if (!server.listen("127.0.0.1", 9090))
    {
        std::cerr << "Failed to start server\n";
        return 1;
    }

    return 0;
}
void updateBook(const httplib::Request& req,
    httplib::Response& res,
    std::vector<Book>& books)
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

void getBooks(const httplib::Request& req, httplib::Response& res, std::vector<Book>& books)
{

    json result = json::array();
    for (const auto& book : books)
    {
        result.push_back({
                {"id", book.id},
                {"title", book.name},
                {"author", book.author}
            });
    }
    res.status = 200;
    res.set_content(
        result.dump(),
        "application/json"
    );
}
void addBook(const httplib::Request& req,
    httplib::Response& res,
    std::vector<Book>& books,
    int& nextBookId)
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
void deleteBook(const httplib::Request& req, httplib::Response& res, std::vector<Book>& books)
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
