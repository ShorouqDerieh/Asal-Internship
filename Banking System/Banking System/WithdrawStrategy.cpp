#include "WithdrawStrategy.h"
#include "account.h"
void WithdrawStrategy::operation(OperationContext& context) {
    context.account.withdraw(context.amount,context.bank);
}
