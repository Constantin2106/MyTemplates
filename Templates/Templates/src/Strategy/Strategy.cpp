

#include <iostream>

#include "Strategy.h"

bool Strategy_1::Execute()
{ 
    bool ok = true; 
    std::cout << "Strategy 1 Execute" << std::endl;
    return true; 
}

bool Strategy_2::Execute()
{
    bool ok = true;
    std::cout << "Strategy 2 Execute" << std::endl;
    return true;
}

bool ExecuteStrategy::Execute()
{
    std::cout << "Execute Strategy Execute" << std::endl;
    return m_strategy->Execute();
}