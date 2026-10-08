#pragma once
#include<atomic>
class Bank {
private:
    std::atomic <int> totalDeposits;
    std::atomic<int> totalWithdrawals;
    std::atomic<int> totalTransfers;

public:
    Bank() : totalDeposits(0), totalWithdrawals(0), totalTransfers(0) {}
   const int getTotalDeposits();
   const int getTotalWithdrawals();
   const int getTotalTransfers();
    void incTotalDeposits();
    void incTotalWithdrawals();
    void incTotalTransfers();
};
