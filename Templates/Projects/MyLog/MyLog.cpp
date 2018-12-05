// MyLog.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <vld.h>

#include "src\Logger.h"
#include "src\LogUtils.h"

#include <conio.h>
#include <iostream>

int main()
{
    Record rec;
    auto str = FormatRecord(rec, "${year}-${mon}-${day}_${h}-${m}-${s}.log");
    std::cout << str.c_str() << std::endl;

    //UseLogger(ConsoleLogger);
    bool isSet = UseLogger(FileLogger);
    std::cout << "First Log setting: " << std::boolalpha << isSet << std::endl;
    isSet = UseLogger(FileLogger);
    std::cout << "Second Log setting: " << std::boolalpha << isSet << std::endl;
 
    int val = 10;
    LogError("Value is: %d", val);
    LogWarn("Value is: %d", val);
    LogInfo("Value is: %d", val);
    LogDebug("Value is: %d", val);

    LogErrorIf(val == 10, "Value is: %d", val);
    LogWarnIf(val == 10, "Value is: %d", val);
    LogInfoIf(val != 10, "Value is: %d", val);
    LogDebugIf(val == 10, "Value is: %d", val);



    _getch();

    return 0;
}
