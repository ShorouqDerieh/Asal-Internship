#pragma once
#include<string>
class Filesystem {
    public:
    void createFile(const std::string& fileName);
    void writeFile(const std::string& name, const std::string& text);
    void appendToFile(const std::string& name, const std::string& text);
    void ReadFile(const std::string& name);
    void copyFile(const std::string& src, const std::string& dest);
    void rename(const std::string& old, const std::string& newName);
    void deleteFile(const std::string& name);
    void search(const std::string& name, const std::string& text);
    void displayInfo(const std::string& name);
    void displayMenu()const;
    std::string getFileName(const std::string& prompt);
    private:
    bool fileExists(const std::string& name);
};
