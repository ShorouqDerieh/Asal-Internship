#pragma once
class Account;
class Bank;
struct OperationContext {
    Account& account;
    Bank& bank;
    double amount;
    Account* destination;
};
class OperationStrategy {
public:
    virtual void operation(OperationContext& context) = 0;
    virtual ~OperationStrategy() = default;
};
