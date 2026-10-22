#include <iostream>
#include<algorithm>
#include "bank.h"
void bank::searchAccount(int accountNumber)
{
    auto it = std::find_if(
        accounts.begin(),
        accounts.end(),
        [&](const Account& account)
        {
            return account.accountNumber == accountNumber;
        }
    );
    if (it == accounts.end())
    {
        std::cout << "Account not found.\n";
        return;
    }
    std::cout << "Account Number: " << accountNumber << "\n" <<
        "Holder Name: " << it->holderName << "\n" <<
        "Balance: " << it->balance << "\n" <<
        "Account Type: " << it->accountType << "\n";
    if (it->accountType == "Savings")
    {
        auto it2 = std::find_if(
            savingsAccounts.begin(),
            savingsAccounts.end(),
            [&](const SavingsAccount& account)
            {
                return account.accountNumber == accountNumber;
            }
        );
        if (it2 != savingsAccounts.end())
        {
            std::cout << "Interest Rate: "
                << it2->interestRate << "%\n";
        }
    }
    else if (it->accountType == "Checking")
    {
        auto it2 = std::find_if(
            checkingAccounts.begin(),
            checkingAccounts.end(),
            [&](const CheckingAccount& account)
            {
                return account.accountNumber == accountNumber;
            }
        );
        if (it2 != checkingAccounts.end())
        {
            std::cout << "Overdraft Limit: "
                << it2->overdraftLimit << "\n";
        }
    }
    std::cout << "\n";
}
void bank::displayAllAccounts()
{
    std::cout << "===== ALL ACCOUNTS =====\n";
       for (auto & account : accounts)
       {
           searchAccount(account.accountNumber);
           std::cout << "\n";
       }
}
void bank::addAccount(const Account& account, double value)
{
    auto it = std::find_if(
        accounts.begin(),
        accounts.end(),
        [&](const Account& acc)
        {
            return acc.accountNumber == account.accountNumber;
        }
    );
    if (it != accounts.end())
    {
        std::cout << "Error: Account number already exists.\n";
        return;
    }
    if (account.accountType == "Savings")
    {
        savingsAccounts.push_back({ account.accountNumber,value });
        accounts.push_back(account);
    }
    else if(account.accountType == "Checking")
    {
        checkingAccounts.push_back({ account.accountNumber,value });
        accounts.push_back(account);
    }
    else
    {
        std::cout << "Error: Invalid account type.\n";
        return;
    }
    std::cout << "Account created successfully.\n";
}
