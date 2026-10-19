#include "CheckBalanceStrategy.h"
#include "account.h"
void CheckBalanceStrategy::operation(OperationContext& context) {
    context.account.checkBalance();
}
