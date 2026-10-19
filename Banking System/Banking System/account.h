#pragma once
#include<mutex>
#include<atomic>
#include<vector>
#include<string>
#include<condition_variable>
class Bank;
class Account {
public:
    int id;
    double balance;
    std::mutex mtx;
    Account(int id, double balance) : id(id), balance(balance) {}
    void deposit(double amount, Bank& bank);
    void withdraw(double amount, Bank& bank);
    void transfer(Account& to, double amount, Bank& bank);
    void checkBalance()const;
    std::string generateStatement()const;
private:
    std::condition_variable cv;
    std::vector<std::string> history;
};
