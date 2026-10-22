#include "filesystem.h"
#include <iostream>
#include <limits>
#include <string>
int main()
{
    int choice;
    std::string name;
    std::string text;
    std::string secondName;
    Filesystem filemanager;
    do {
        filemanager.displayMenu();
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        switch (choice) {
            case 0:
                std::cout << "Thank you for using C++ File Manager!\n";
                break;
            case 1:
                filemanager.createFile(filemanager.getFileName("Please enter file name: "));
                break;
            case 2:
                name = filemanager.getFileName("Please enter file name: ");
                std::cout << "Please Enter Text: ";
                std::getline(std::cin, text);
                filemanager.writeFile(name, text);
                break;
            case 3:
                name = filemanager.getFileName("Please enter file name: ");
                std::cout << "Please Enter Text: ";
                std::getline(std::cin, text);
                filemanager.appendToFile(name, text);
                break;
            case 4:
                filemanager.ReadFile(filemanager.getFileName("Please enter file name: "));
                break;
            case 5:
                filemanager.copyFile(filemanager.getFileName("Source filename: "), filemanager.getFileName("Destination filename: "));
                break;
            case 6:
                name = filemanager.getFileName("Please enter old file name: ");
                secondName = filemanager.getFileName("Please enter new file name: ");
                filemanager.rename(name, secondName);
                break;
            case 7:
                filemanager.deleteFile(filemanager.getFileName("Please enter file name: "));
                break;
            case 8:
                name = filemanager.getFileName("Please enter file name: ");
                std::cout << "enter text to search: ";
                std::getline(std::cin, text);
                filemanager.search(name, text);
                break;
            case 9:
                filemanager.displayInfo(filemanager.getFileName("Please enter file name: "));
                break;
            default:
                std::cout << "Invalid option!\n";
                break;
        }

    } while (choice != 0);

}
