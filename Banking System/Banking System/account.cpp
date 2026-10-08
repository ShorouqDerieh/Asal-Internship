#include<iostream>
#include<thread>
#include<condition_variable>
#include "account.h"
#include "bank.h"
void Account::deposit(double amount, Bank& bank) {
        if (amount <= 0) {
            std::cout << "Deposit amount must be positive." << std::endl;
            return;
        }
        {
        std::lock_guard<std::mutex>lock(mtx);
        balance += amount;
        history.push_back("Deposited: " + std::to_string(amount) + ", New Balance: " + std::to_string(balance));
    }
        bank.incTotalDeposits();
    cv.notify_all();
}
void Account::withdraw(double amount, Bank& bank) {
    if (amount <= 0) {
        std::cout << "Withdrawal amount must be positive." << std::endl;
        return;
    }
    std::unique_lock<std::mutex>lock(mtx);
       cv.wait(lock, [&] {
            return balance >= amount;
            });
           balance -= amount;
           history.push_back("Withdrew: " + std::to_string(amount) + ", New Balance: " + std::to_string(balance));
       bank.incTotalWithdrawals();
}
void Account::transfer( Account& to, double amount, Bank& bank) {
        if (amount <= 0) {
            std::cout << "Transfer amount must be positive." << std::endl;
            return;
        }
        if (this == &to)
        {
            std::cout << "Cannot transfer to the same account.\n";
            return;
        }
        {
            std::scoped_lock lock(this->mtx, to.mtx);
            if (this->balance < amount) {
                std::cout << "Insufficient balance in account ID: " << this->id << std::endl;
                return;
            }
            to.balance += amount;
            this->balance -= amount;
            history.push_back("Transferred: " + std::to_string(amount) + ", New Balance: " + std::to_string(balance));
            to.history.push_back("Received: " + std::to_string(amount) + ", New Balance: " + std::to_string(to.balance));
        }
    to.cv.notify_all();
    bank.incTotalTransfers();
}
const void Account::checkBalance() {
    std::lock_guard<std::mutex>lock(mtx);
    std::cout << "Account ID: " << id << ", Balance: " << balance << std::endl;
}
 std::string Account::generateStatement() {
    std::lock_guard<std::mutex>lock(mtx);
    std::string statement = "Account ID: " + std::to_string(id) + " Statement:\n";
    for (const auto& entry : history) {
        statement += entry + "\n";
    }
    return statement;
}


