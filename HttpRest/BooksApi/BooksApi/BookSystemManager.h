#pragma once
#include <vector>
#include "Book.h"
#include "httplib.h"
class BookSystemManager
{
private:
    std::vector<Book> books;
    int nextBookId ;

public:
    explicit BookSystemManager(std::vector<Book> initialBooks);

    void getBooks(const httplib::Request& req,httplib::Response& res);
    void addBook(const httplib::Request& req,httplib::Response& res);
    void updateBook(const httplib::Request& req,httplib::Response& res);
    void deleteBook(const httplib::Request& req,httplib::Response& res);
};
