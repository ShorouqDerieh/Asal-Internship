#pragma once
#include "OperationStrategy.h"
class TransferStrategy :public OperationStrategy {
public:
    void operation(OperationContext& context) override;
};
