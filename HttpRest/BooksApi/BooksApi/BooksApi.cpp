#include <iostream>
#include <vector>
#include <string>
#include <utility>
#include "httplib.h"
#include "json.hpp"
#include "Book.h"
#include "BookSystemManager.h"
using json = nlohmann::json;

int main() {
     std::vector<Book> initialBooks = {
        {1, "Clean Code", "Robert C. Martin"},
        {2, "The Pragmatic Programmer", "Andrew Hunt"}
    };

    BookSystemManager manager(std::move(initialBooks));
    httplib::Server server;
    server.Get("/books",
        [&](const httplib::Request& req,
            httplib::Response& res) {
                manager.getBooks(req, res);
        });
    server.Post("/books",
        [&](const httplib::Request& req,
            httplib::Response& res) {
                manager.addBook(req, res);
        }
    );
    server.Put("/books/:id",
        [&](const httplib::Request& req,
            httplib::Response& res)
        {
            manager.updateBook(req, res);
        }
    );
    server.Delete("/books/:id", [&](const httplib::Request& req,
        httplib::Response& res) {
            manager.deleteBook(req, res);
        });

    std::cout << "Server running at http://localhost:9090\n";

    if (!server.listen("127.0.0.1", 9090))
    {
        std::cerr << "Failed to start server\n";
        return 1;
    }

    return 0;
}
