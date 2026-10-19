#include "DepositStrategy.h"
#include "account.h"
void DepositStrategy::operation( OperationContext & context ) {
    context.account.deposit(context.amount, context.bank);
}
