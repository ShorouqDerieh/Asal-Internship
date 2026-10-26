#include <iostream>
#include<vector>
#include<string>
#include "httplib.h"
#include "json.hpp"
int main() {
    using json = nlohmann::json;
    struct Book {
        int id;
        std::string name;
        std::string author;
    };
    httplib::Server server;
    std::vector<Book> books = {
    {1, "Clean Code", "Robert C. Martin"},
    {2, "The Pragmatic Programmer", "Andrew Hunt"}
    };
    int nextBookId = 3;
    server.Get("/books",
        [&](const httplib::Request& req,
            httplib::Response& res) {

                res.status = 200;
                json result = json::array();
                for(const auto& book:books)
                {
                    result.push_back({
                            {"id", book.id},
                            {"title", book.name},
                            {"author", book.author}
                        });
                }
                res.set_content(
                   result.dump(),
                    "application/json"
                );
        });
    server.Post("/books",
        [&](const httplib::Request& req,
            httplib::Response& res) {
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
    );
    server.Put("/books/:id",
        [&](const httplib::Request& req,
            httplib::Response& res) {
                json body = json::parse(req.body);
                int id = std::stoi(req.path_params.at("id"));
                json response;
                for (auto& book : books) {
                    if (book.id == id) {
                        book.author = body.at("author").get<std::string>();
                        book.name = body.at("name").get<std::string>();
                       response = {
                       {"id", id },
                       {"name", body.at("name").get<std::string>()},
                       {"author", body.at("author").get<std::string>()}
                        };
                       res.status = 200;
                       res.set_content(
                           response.dump(),
                           "application/json"
                       );
                       return;
                    }
                }
                res.status = 404;
                res.set_content(
                    R"({"error":"Book not found"})",
                    "application/json"
                );
        }
        );
    server.Delete("/books/:id", [&](const httplib::Request& req,
        httplib::Response& res) {
            int id = std::stoi(req.path_params.at("id"));
            for (auto it = books.begin(); it != books.end(); ++it) {
                if (it->id == id) {
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

        });
    std::cout << "Server running at http://localhost:9090\n";

    if (!server.listen("127.0.0.1", 9090)) {
        std::cerr << "Failed to start server\n";
        return 1;
    }

    return 0;
}
