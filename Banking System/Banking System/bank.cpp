#include "bank.h"
void Bank::incTotalDeposits() {
    totalDeposits++;
}
void Bank::incTotalWithdrawals() {
    totalWithdrawals++;
}
void Bank::incTotalTransfers() {
    totalTransfers++;
}
const int Bank::getTotalDeposits() {
    return totalDeposits.load();
}
const int Bank::getTotalWithdrawals() {
    return totalWithdrawals.load();
}
const int Bank::getTotalTransfers() {
    return totalTransfers.load();
}
