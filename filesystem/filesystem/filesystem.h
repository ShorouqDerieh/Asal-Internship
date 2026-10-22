#pragma once
#include<string>
#include <string_view>
class Filesystem {
    public:
    void createFile(std::string_view fileName);
    void writeFile(std::string_view name, std::string_view text);
    void appendToFile(std::string_view name, std::string_view text);
    void ReadFile(std::string_view name);
    void copyFile(std::string_view src, std::string_view dest);
    void rename(std::string_view old, std::string_view newName);
    void deleteFile(std::string_view name);
    void search(std::string_view name, std::string_view text);
    void displayInfo(std::string_view name);
    void displayMenu()const;
    std::string getFileName(std::string_view prompt);
    private:
    bool fileExists(std::string_view name);
};
