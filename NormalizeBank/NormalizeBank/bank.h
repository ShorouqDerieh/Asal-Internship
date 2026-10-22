#pragma once
#include <vector>
#include "Account.h"
#include"CheckingAccount.h"
#include "SavingsAccount.h"
class bank {
private:
    std::vector<Account> accounts;
    std::vector<SavingsAccount> savingsAccounts;
    std::vector<CheckingAccount> checkingAccounts;
public:
    void searchAccount(int accountNumber);
    void displayAllAccounts();
    void addAccount(const Account& account, double value);

};
