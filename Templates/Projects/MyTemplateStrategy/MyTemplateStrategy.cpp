// MyTemplateStrategy.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "vld.h"

#include <iostream>

#include "StrategyClasses.h"
#include "StrategyTemplate.h"

using namespace std;
using namespace StrategyClasses;

int main()
{
    /*std::unique_ptr<Strategy<Strategy_1>> strategy_1;
    strategy_1.reset(new Strategy<Strategy_1>("111"));
    auto strategy_2 = std::make_unique<Strategy<Strategy_2>>("222");
    strategy_1->Run();
    strategy_2->Run();*/

    Strategy<Strategy_1>("111").Run();

    Handle<Strategy_1>("111");
    Handle<StrategyClasses::Strategy_2>("222");
}

