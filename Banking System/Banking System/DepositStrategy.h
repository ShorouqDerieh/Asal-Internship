#pragma once
#include "OperationStrategy.h"
class DepositStrategy :public OperationStrategy {
public:
    void operation(OperationContext& context) override;
};
