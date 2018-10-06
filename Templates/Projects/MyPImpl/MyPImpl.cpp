// MyPImpl.cpp : Defines the entry point for the console application.
//
#include <vld.h>

#include <conio.h>
#include <iostream>

#include "src/PImp.h"

int main()
{
    std::cout << "Create PImpl" << std::endl;

    PImpl pimpl;

    pimpl.Set();
    pimpl.Check();
    pimpl.Execute();

    _getch();

    return 0;
}

