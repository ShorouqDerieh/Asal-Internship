#pragma once
#include "OperationStrategy.h"
class CheckBalanceStrategy :public OperationStrategy {
public:
    void operation(OperationContext& context) override;
};
