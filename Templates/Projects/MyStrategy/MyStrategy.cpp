// MyStrategy.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include <conio.h>
#include <iostream>
#include <memory>

#include "src\Strategy.h"

int main()
{
    auto st1 = new Strategy_1();
    auto st2 = new Strategy_2();
    auto stExecutor = new StrategyExecutor();
    std::cout << std::endl;

    stExecutor->SetStrategy(st1);
    stExecutor->Execute();
    std::cout << std::endl;

    _getch();

    stExecutor->SetStrategy(st2);
    stExecutor->Execute();
    std::cout << std::endl;

    _getch();

    delete stExecutor;

    return 0;
}

