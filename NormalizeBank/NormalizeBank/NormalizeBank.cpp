#include <iostream>
#include <vector>
#include "bank.h"
int main()
{
    bank myBank;
    myBank.addAccount(Account({ 1001,"Ahmed Ali",5000,"Savings" }), 5);
    myBank.addAccount(Account({ 1002,"Sara Omar",3000,"Checking" }), 1000);
    myBank.addAccount(Account({ 1003,"Omar Hassan",7000,"Savings" }), 4);
    myBank.addAccount(Account({ 1004,"Omar l",7000,"Business" }), 4);
    myBank.searchAccount(1001);
    myBank.searchAccount(1002);
    myBank.searchAccount(1003);
    myBank.searchAccount(2000);
    myBank.displayAllAccounts();
    myBank.addAccount(Account({ 1001,"Ahmed d",7000,"Savings" }), 7);


}
