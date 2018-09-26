
#include <conio.h>
#include <iostream>

#include "Strategy.h"

void StrategyTest()
{
    ExecuteStrategy* exStrartegy = new ExecuteStrategy(new Strategy_1());
    std::cout << std::endl;

    exStrartegy->Execute();
    std::cout << std::endl;
     _getch();

    delete exStrartegy;
    std::cout << std::endl;

    exStrartegy = new ExecuteStrategy(new Strategy_2());
    std::cout << std::endl;

    exStrartegy->Execute();
    std::cout << std::endl;
    _getch();

    delete exStrartegy;
}