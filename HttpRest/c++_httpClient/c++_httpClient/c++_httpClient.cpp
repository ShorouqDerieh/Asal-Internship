
#include <iostream>
#include <string>
#include "httplib.h"
#include "json.hpp"

using json = nlohmann::json;

int main()
{
    httplib::Client client("http://127.0.0.1:9090");
    auto result = client.Get("/books");

    if (!result)
    {
        std::cout << "GET request failed\n";
    }

    std::cout << "=== GET ALL BOOKS ===\n" <<
        "Status: " << result->status << '\n' <<
        "Body: " << result->body << "\n\n";

    std::string body = R"({
        "name": "Design Patterns",
        "author": "Erich Gamma"
    })";

    result = client.Post("/books", body, "application/json");

    if (!result)
    {
        std::cout << "POST request failed\n";
    }

    std::cout << "=== POST BOOK ===\n"<<
         "Status: " << result->status << '\n'<<
         "Body: " << result->body << "\n\n";

    if (result->status != 201)
    {
        std::cout << "Failed to create book\n";
    }

    int bookId;
    json response = json::parse(result->body);
    bookId = response.at("id").get<int>();
    std::string bookPath = "/books/" + std::to_string(bookId);
    std::string updatedBody = R"({
        "name": "Design Patterns Updated",
        "author": "Erich Gamma"
    })";

    result = client.Put(bookPath, updatedBody, "application/json");

    if (!result)
    {
        std::cout << "PUT request failed\n";
    }

    std::cout << "=== PUT BOOK ===\n"<<
     "Status: " << result->status << '\n'<<
     "Body: " << result->body << "\n\n";

    if (result->status != 200)
    {
        std::cout << "Failed to update book\n";
    }

    result = client.Delete(bookPath);

    if (!result)
    {
        std::cout << "DELETE request failed\n";
    }

    std::cout << "=== DELETE BOOK ===\n"<<
     "Status: " << result->status << '\n';

    if (result->status != 204)
    {
        std::cout << "Failed to delete book\n";
    }
    return 0;
}
