#include <iostream>
#include <thread>
#include<vector>
#include<string>
#include<future>
#include<random>
#include "account.h"
#include "bank.h"
void runATM(std::vector<Account*>& accounts, Bank& bank,int id)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> operationDist(0, 3);
    std::uniform_int_distribution<int> accountDist(0, accounts.size() - 1);
    std::uniform_int_distribution<int> amountDist(10, 100);

    for (int i = 0; i < 10; i++)
    {
        int operation = operationDist(gen);
        int accountIndex = accountDist(gen);
        double amount = amountDist(gen);

        Account& account = *accounts[accountIndex];

        switch (operation)
        {
        case 0:
            account.deposit(amount, bank);
            break;

        case 1:
            account.withdraw(amount, bank);
            break;

        case 2:
        {
            int toIndex;

            do {
                toIndex = accountDist(gen);
            } while (toIndex == accountIndex);

            account.transfer(*accounts[toIndex], amount, bank);
            break;
        }

        case 3:
            account.checkBalance();
            break;
        }
    }

}
int main()
{
    Bank bank;
    std::vector<std::thread>threads;
    std::vector<Account*>accounts;
    Account account1(1, 1000.0);
    Account account2(2, 500.0);
    Account account3(3, 150.0);
    accounts.push_back(&account1);
    accounts.push_back(&account2);
    accounts.push_back(&account3);

    for (int i = 1; i <= 10; i++)
    {
        threads.emplace_back(runATM, std::ref(accounts), std::ref(bank),i);
    }
    for (std::thread& t : threads)
    {
        t.join();
    }
    std::cout << "\n===== Final Balances =====\n";

    account1.checkBalance();
    account2.checkBalance();
    account3.checkBalance();

    std::cout << "\n===== Statistics =====\n";

    std::cout << "Total Deposits: "
        << bank.getTotalDeposits() << std::endl;

    std::cout << "Total Withdrawals: "
        << bank.getTotalWithdrawals() << std::endl;

    std::cout << "Total Transfers: "
        << bank.getTotalTransfers() << std::endl;

    std::cout << "\n===== Account Statements =====\n";

    auto future1 = std::async(
        std::launch::async,
        &Account::generateStatement,
        &account1
    );

    auto future2 = std::async(
        std::launch::async,
        &Account:: generateStatement,
        &account2
    );

    auto future3 = std::async(
        std::launch::async,
        &Account::generateStatement,
        &account3
    );

    std::cout << future1.get() << std::endl;
    std::cout << future2.get() << std::endl;
    std::cout << future3.get() << std::endl;

    return 0;
}

