#pragma once
#include<memory>
#include "OperationStrategy.h"
class ATMContext {
private:
    std::unique_ptr<OperationStrategy> strategy;
public:
    explicit ATMContext(std::unique_ptr<OperationStrategy>s) :strategy(std::move(s)) {}
    void operate(OperationContext& context);
    void setStrategy(std::unique_ptr<OperationStrategy>s);
};
