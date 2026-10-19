#include "TransferStrategy.h"
#include "account.h"
void TransferStrategy::operation(OperationContext& context) {
    context.account.transfer(*context.destination,context.amount,context.bank);
}
