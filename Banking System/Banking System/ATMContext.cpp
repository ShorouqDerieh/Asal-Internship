#include "ATMContext.h"
void ATMContext::operate(OperationContext& context)
{
    strategy->operation(context);
}
void ATMContext::setStrategy(std::unique_ptr<OperationStrategy>s)
{
    strategy = std::move(s);
}
