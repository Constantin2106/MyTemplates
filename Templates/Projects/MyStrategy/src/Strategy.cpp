

#include <iostream>

#include "Strategy.h"

bool Strategy_1::Execute()
{ 
    bool ok = true; 
    std::cout << "Strategy 1 Executed" << std::endl;
    return true; 
}

bool Strategy_2::Execute()
{
    bool ok = true;
    std::cout << "Strategy 2 Executed" << std::endl;
    return true;
}

StrategyExecutor::StrategyExecutor() : m_strategy(nullptr)
{
    std::cout << "Constructor Strategy Executor" << std::endl;
}
StrategyExecutor::~StrategyExecutor()
{
    std::cout << "Destructor Execute Strategy" << std::endl;
}

bool StrategyExecutor::SetStrategy(BaseStrategy* _strategy)
{
    if(!_strategy)
    {
        return false;
    }

    m_strategy = _strategy;

    return true;
}

bool StrategyExecutor::Execute()
{
    std::cout << "Strategy Executor" << std::endl;
    return m_strategy->Execute();
}
