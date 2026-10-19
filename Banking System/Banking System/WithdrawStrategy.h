#pragma once
#include "OperationStrategy.h"
class WithdrawStrategy :public OperationStrategy {
public:
    void operation(OperationContext& context) override;
};
