// MyLog.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <vld.h>

#include "src\Logger.h"

#include <conio.h>
#include <iostream>
#include <atomic>

std::atomic<ILogger*> g_logger;// = new ConsoleLogger();

int main()
{
    ConsoleLogger log;
    g_logger = &log;
    int val = 10;
    LogError("Value is: %d", val);

    _getch();
}
