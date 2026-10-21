#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include<limits>
#include <vector>
namespace fs = std::filesystem;
void createFile(const std::string& fileName);
void writeFile(const std::string& name,const std::string& text);
void appendToFile(const std::string& name, const std::string& text);
void ReadFile(const std::string& name);
void copyFile(const std::string& src, const std::string& dest);
void rename(const std::string& old, const std::string& newName);
void deleteFile(const std::string& name);
void search(const std::string& name, const std::string& text);
void displayInfo(const std::string& name);
std::string getFileName(const std::string& prompt);
bool fileExists(const std::string& name);
int main()
{
    int choice;
    std::string name;
    std::string text;
    std::string secondName;
    do {
        std::cout << "===================================\n" <<
            "       C++ FILE MANAGER\n"<<
            "===================================\n"
            "1. Create a new file\n" <<
            "2. Write to a file\n" <<
            "3. Append to a file\n" <<
            "4. Read a file\n" <<
            "5. Copy a file\n" << 
            "6. Rename a file\n" << 
            "7. Delete a file\n" <<
            "8. Search text in a file\n" <<
            "9. Display file information\n"<<
            "0. Exit\n" << 
            "===================================\n"<<
            "Enter your choice:";
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
        case 0:
            std::cout << "Thank you for using C++ File Manager!\n";
            break;
        case 1:
            createFile(getFileName("Please enter file name: "));
            break;
        case 2:
            name = getFileName("Please enter file name: ");
            std::cout << "Please Enter Text: ";
            std::getline(std::cin, text);
            writeFile(name, text);
            break;
        case 3:
            name = getFileName("Please enter file name: ");
            std::cout << "Please Enter Text: ";
            std::getline(std::cin, text);
            appendToFile(name, text);
            break;
        case 4:
            ReadFile(getFileName("Please enter file name: "));
            break;
        case 5:
            copyFile(getFileName("Source filename: ") , getFileName("Destination filename: "));
            break;
        case 6:
            name = getFileName("Please enter old file name: ");
            secondName = getFileName("Please enter new file name: ");
            rename(name, secondName);
            break;
        case 7:
            deleteFile(getFileName("Please enter file name: "));
            break;
        case 8:
            name = getFileName("Please enter file name: ");
            std::cout << "enter text to search: ";
            std::getline(std::cin, text);
            search(name, text);
            break;
        case 9:
            displayInfo(getFileName("Please enter file name: "));
            break;
        default:
            std::cout << "Invalid option!\n";
            break;
        }
        
    } while (choice != 0);

}
void createFile(const std::string & fileName)
{
    if (fs::exists(fileName))
    {
        std::cout << "File already exists.\n";
        return;
    }
    std::ofstream file(fileName);
    if (!file)
    {
        std::cout << "Failed to create file\n";
        return;
    }
    std::cout << "File created successfully\n";
}
void writeFile(const std::string& name, const std::string& text)
{
    if (!fileExists(name))
        return;
    std::ofstream file(name);
    if (!file)
    {
        std::cout << "Failed to open file\n";
        return;
    }
        file << text;
        if (!file)
        {
            std::cout << "Failed to write to file\n";
            return;
        }
        std::cout << "Write to file successfully\n";
}
void appendToFile(const std::string& name, const std::string& text)
{
    if (!fileExists(name))
        return;
    std::ofstream file(name,std::ios::app);
    if (!file)
    {
        std::cout << "Failed to open file\n";
        return;
    }
        file << text;
        if (!file)
        {
            std::cout << "Failed to append to file\n";
            return;
        }
        std::cout << "Append to file successfully\n";
}
void ReadFile(const std::string& name)
{
    if (!fileExists(name))
        return;
    std::string s;
    std::ifstream file(name, std::ios::in);
    if (!file)
    {
        std::cout << "Failed to open file\n";
        return;
    }
    std::cout << "----- File Contents -----\n";
    while (std::getline(file, s))
        std::cout << s << std::endl;
    std::cout << "--------------------------\n";
}
void copyFile(const std::string& src, const std::string& dest)
{
    if (!fileExists(src))
        return;
    try
    {
        if (fs::copy_file(src, dest))
        {
            std::cout << "File copied successfully\n";
        }
        else
        {
            std::cout << "File was not copied\n";
        }
    }
    catch (const fs::filesystem_error& e)
    {
        std::cout << "Copy failed: " << e.what() << '\n';
    }
}
void rename(const std::string& old, const std::string& newName)
{
    if (!fileExists(old))
        return;
    try {
        fs::rename(old, newName);
       std::cout << "File renamed successfully.\n";
    }
    catch (const fs::filesystem_error& e)
    {
        std::cout << "Failed to rename file: " << e.what() << '\n';
    }
}
void deleteFile(const std::string& name)
{
    char choice;
    std::cout << "Are you sure you want to delete " << name << "? (Y / N) :";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (choice == 'Y'||choice=='y')
    {
        if (!fileExists(name))
            return;
        try {
            if (fs::remove(name))
                std::cout << "File removed successfully.\n";
            else
                std::cout << "File was not removed.\n";
        }
        catch (const fs::filesystem_error& e)
        {
            std::cout << "Failed to delete file: " << e.what() << '\n';
        }
    }
}
void search(const std::string& name, const std::string& text)
{
    if (!fileExists(name))
        return;
    if (text.empty())
    {
        std::cout << "Search text cannot be empty\n";
        return;
    }
    std::vector<int>lines;
    std::ifstream file(name);
    if (!file)
    {
        std::cout << "Failed to open file\n";
        return;
    }
    int lineNumber = 0;
    std::string line;
    bool found = false;
    while (std::getline(file, line))
    {
        lineNumber++;

        if (line.find(text) != std::string::npos)
        {
            found = true;
            lines.push_back(lineNumber);
        }
    }
    found?std::cout << "Text Found!\n":std::cout<<"Text not found!\n";
    for (auto& num : lines)
    {
        std::cout << "Found on line: " <<
            num<<"\n";
    }
}
void displayInfo(const std::string& name)
{
    if (!fileExists(name))
        return;
    std::cout << "Filename: " <<
        name <<"\n"<<
        "File exists: " <<
        "Yes\n" <<
        "File size: " <<
        fs::file_size(name)<<" bytes\n";
}
std::string getFileName(const std::string& prompt)
{
    std::string name;
    std::cout << prompt;
    std::getline(std::cin, name);
    return name;
}
bool fileExists(const std::string& name)
{
    if (!fs::exists(name))
    {
        std::cout << "File not found\n";
        return false;
    }

    return true;
}
