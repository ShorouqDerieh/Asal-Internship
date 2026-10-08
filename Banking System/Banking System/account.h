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
    const void checkBalance();
    std::string generateStatement();
private:
    std::condition_variable cv;
    std::vector<std::string> history;
};
